#include "test_runner.h"
#include <limits>
#include <random>
#include <unordered_set>

using namespace std;

struct Address {
    string city, street;
    int building;

    bool operator==(const Address& other) const {
        // ���������� ��������
        return tie(city, street, building) ==
               tie(other.city, other.street, other.building);
    }
};

struct Person {
    string name;
    int height;
    double weight;
    Address address;

    bool operator==(const Person& other) const {
        // ���������� ��������
        return tie(name, height, weight, address) ==
               tie(other.name, other.height, other.weight, other.address);
    }
};

struct AddressHasher {
    // ���������� ���������
    size_t operator()(const Address& address) const
    {
        const size_t x = 911;
        hash<string> h_s;
        hash<int> h_i;
        hash<double> h_d;
        return h_s(address.city) * x * x * x +
               h_s(address.street) * x * x + 
               h_i(address.building) * x;
    }
};

struct PersonHasher {
  // ���������� ���������
    size_t operator()(const Person& person) const
    {
        const size_t x = 911;
        hash<string> h_s;
        hash<int> h_i;
        hash<double> h_d;
        AddressHasher h_a;
        return h_s(person.name) * x * x * x * x +
               h_i(person.height) * x * x * x + 
               h_d(person.weight) * x * x +
               h_a(person.address) * x;
    }
};

// ������������� ��������:
// $ sort -R /usr/share/dict/propernames | head -n 100
//
// http://www.freebsd.org/cgi/cvsweb.cgi/~checkout~/src/share/dict/propernames
const vector<string> WORDS = {
  "Kieran", "Jong", "Jisheng", "Vickie", "Adam", "Simon", "Lance",
  "Everett", "Bryan", "Timothy", "Daren", "Emmett", "Edwin", "List",
  "Sharon", "Trying", "Dan", "Saad", "Kamiya", "Nikolai", "Del",
  "Casper", "Arthur", "Mac", "Rajesh", "Belinda", "Robin", "Lenora",
  "Carisa", "Penny", "Sabrina", "Ofer", "Suzanne", "Pria", "Magnus",
  "Ralph", "Cathrin", "Phill", "Alex", "Reinhard", "Marsh", "Tandy",
  "Mongo", "Matthieu", "Sundaresan", "Piotr", "Ramneek", "Lynne", "Erwin",
  "Edgar", "Srikanth", "Kimberly", "Jingbai", "Lui", "Jussi", "Wilmer",
  "Stuart", "Grant", "Hotta", "Stan", "Samir", "Ramadoss", "Narendra",
  "Gill", "Jeff", "Raul", "Ken", "Rahul", "Max", "Agatha",
  "Elizabeth", "Tai", "Ellen", "Matt", "Ian", "Toerless", "Naomi",
  "Rodent", "Terrance", "Ethan", "Florian", "Rik", "Stanislaw", "Mott",
  "Charlie", "Marguerite", "Hitoshi", "Panacea", "Dieter", "Randell", "Earle",
  "Rajiv", "Ted", "Mann", "Bobbie", "Pat", "Olivier", "Harmon",
  "Raman", "Justin"
};

void TestSmoke() {
  vector<Person> points = {
    {"John", 180, 82.5, {"London", "Baker St", 221}},
    {"Sherlock", 190, 75.3, {"London", "Baker St", 221}},
  };

  unordered_set<Person, PersonHasher> point_set;
  for (const auto& point : points) {
    point_set.insert(point);
  }

  ASSERT_EQUAL(points.size(), point_set.size());
  for (const auto& point : points) {
    ASSERT_EQUAL(point_set.count(point), static_cast<size_t>(1));
  }
}

void TestPurity() {
  Person person = {"John", 180, 82.5, {"London", "Baker St", 221}};
  PersonHasher hasher;

  auto hash = hasher(person);
  for (size_t t = 0; t < 100; ++t) {
    ASSERT_EQUAL(hasher(person), hash);
  }
};

void TestDistribution() {
  auto seed = 42;
  mt19937 gen(seed);

  uniform_int_distribution<int> height_dist(150, 200);
  uniform_int_distribution<int> weight_dist(100, 240);  // [50, 120]
  uniform_int_distribution<int> building_dist(1, 300);
  uniform_int_distribution<int> word_dist(0, WORDS.size() - 1);

  PersonHasher hasher;

  // �������� ����� ������� �� ����� ������� ������� ������
  // (unordered_set, unordered_map ���������� ������� ����� �������
  // � ����������� GCC � Clang, ��� ��������� ����� �������
  // ��������� ��������� �� ����������� ���-�������� � ���� �����������)
  const size_t num_buckets = 2053;

  // �� �����, ����� ����� ����� � ������� ���� ~100'000
  const size_t perfect_bucket_size = 50;
  const size_t num_points = num_buckets * perfect_bucket_size;
  vector<size_t> buckets(num_buckets);
  for (size_t t = 0; t < num_points; ++t) {
    Person person;
    person.name = WORDS[word_dist(gen)];
    person.height = height_dist(gen);
    person.weight = weight_dist(gen) * 0.5;
    person.address.city = WORDS[word_dist(gen)];
    person.address.street = WORDS[word_dist(gen)];
    person.address.building = building_dist(gen);
    ++buckets[hasher(person) % num_buckets];
  }

  // ���������� �������:
  // https://en.wikipedia.org/wiki/Pearson's_chi-squared_test
  //
  // ��������� ����� ������������� ������������� ����� ����� ���������
  // ��������, �� ��� �� ������� ��� ������� �������������� �������������
  double pearson_stat = 0;
  for (auto bucket_size : buckets) {
    size_t size_diff = bucket_size - perfect_bucket_size;
    pearson_stat +=
        size_diff * size_diff / static_cast<double>(perfect_bucket_size);
  }

  // ��������� ������������� ������������� ������� �������� �������
  // �� �������������� ����������� 0.95:
  //
  // ���� ���������� ������ ++buckets[hasher(person) % num_buckets]
  // ��������� ++buckets[gen() % num_buckets], �� � ������������ 95%
  // ASSERT ���� ���������� �������,
  //
  // �.�. ���������� ������� �������������� ������������ �� chi^2
  // � ������ �������� �������, ������ num_buckets - 1,
  // � 95 ���������� ����� ������������� �����:
  // >>> scipy.stats.chi2.ppf(0.95, 2052)
  const double critical_value = 2158.4981036918693;
  ASSERT(pearson_stat < critical_value);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSmoke);
  RUN_TEST(tr, TestPurity);
  RUN_TEST(tr, TestDistribution);

  return 0;
}