#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "test_runner.h"

using namespace std;

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;
  Gender gender;
  bool is_employed;
};

bool operator==(const Person& lhs, const Person& rhs) {
  return lhs.age == rhs.age
    && lhs.gender == rhs.gender
    && lhs.is_employed == rhs.is_employed;
}

ostream& operator<<(ostream& stream, const Person& person) {
  return stream << "Person(age=" << person.age
    << ", gender=" << static_cast<int>(person.gender)
    << ", is_employed=" << boolalpha << person.is_employed << ")";
}

struct AgeStats {
  int total;
  int females;
  int males;
  int employed_females;
  int unemployed_females;
  int employed_males;
  int unemployed_males;
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename iterator_traits<InputIt>::value_type> range_copy(
    range_begin,
    range_end
  );
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
    begin(range_copy), middle, end(range_copy),
    [](const Person& lhs, const Person& rhs) {
      return lhs.age < rhs.age;
    }
  );
  return middle->age;
}

vector<Person> ReadPersons(istream& in_stream = cin) {
  int person_count;
  in_stream >> person_count;
  vector<Person> persons;
  persons.reserve(person_count);
  for (int i = 0; i < person_count; ++i) {
    int age, gender, is_employed;
    in_stream >> age >> gender >> is_employed;
    Person person{
        age,
        static_cast<Gender>(gender),
        is_employed == 1
    };
    persons.push_back(person);
  }
  return persons;
}

AgeStats ComputeStats(vector<Person> persons) {
  //                 persons
  //                |       |
  //          females        males
  //         |       |      |     |
  //      empl.  unempl. empl.   unempl.

  auto females_end = partition(
    begin(persons), end(persons),
    [](const Person& p) {
      return p.gender == Gender::FEMALE;
    }
  );
  auto employed_females_end = partition(
    begin(persons), females_end,
    [](const Person& p) {
      return p.is_employed;
    }
  );
  auto employed_males_end = partition(
    females_end, end(persons),
    [](const Person& p) {
      return p.is_employed;
    }
  );

  return {
       ComputeMedianAge(begin(persons), end(persons)),
       ComputeMedianAge(begin(persons), females_end),
       ComputeMedianAge(females_end, end(persons)),
       ComputeMedianAge(begin(persons),  employed_females_end),
       ComputeMedianAge(employed_females_end, females_end),
       ComputeMedianAge(females_end, employed_males_end),
       ComputeMedianAge(employed_males_end, end(persons))
  };
}

void PrintStats(const AgeStats& stats,
  ostream& out_stream = cout) {
  out_stream << "Median age = "
    << stats.total << endl
    << "Median age for females = "
    << stats.females << endl
    << "Median age for males = "
    << stats.males << endl
    << "Median age for employed females = "
    << stats.employed_females << endl
    << "Median age for unemployed females = "
    << stats.unemployed_females << endl
    << "Median age for employed males = "
    << stats.employed_males << endl
    << "Median age for unemployed males = "
    << stats.unemployed_males << endl;
}

void TestBug1() {
  vector<Person> persons;
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 0);
}

void TestBug2() {
  vector<Person> persons = {
    {31, Gender::MALE, false},
    {40, Gender::FEMALE, true},
    {24, Gender::MALE, true},
  };
  ASSERT_EQUAL(ComputeMedianAge(persons.begin(), persons.end()), 31);
}

void TestBug3() {
  vector<Person> persons = { {31, Gender::MALE, false} };
  ostringstream os;
  os << 1 << " " << 31 << " " << 1 << " " << 0;
  istringstream is(os.str());
  ASSERT_EQUAL(ReadPersons(is), persons);
}

void TestBug4() {
  vector<Person> persons = { {31, Gender::MALE, false} };
  ostringstream os;
  os << 1 << " " << 31 << " " << 1 << " " << 0;
  istringstream is(os.str());
  ASSERT_EQUAL(ReadPersons(is), persons);
}

void TestBug5() {
  vector<Person> persons = {
    {31, Gender::MALE, false},
    {25, Gender::MALE, false},
    {40, Gender::FEMALE, true},
    {50, Gender::FEMALE, false},
    {58, Gender::FEMALE, true},
    {25, Gender::FEMALE, true},
    {26, Gender::FEMALE, true},
    {27, Gender::FEMALE, true},
    {28, Gender::FEMALE, true},
    {24, Gender::MALE, true},
    {20, Gender::FEMALE, true},
    {80, Gender::FEMALE, false},
    {70, Gender::FEMALE, false},
    {78, Gender::MALE, false},
    {60, Gender::MALE, false},
    {54, Gender::MALE, true},
    {21, Gender::MALE, false},
    {78, Gender::MALE, true},
    {10, Gender::FEMALE, false},
    {66, Gender::MALE, true},
    {90, Gender::MALE, true},
    {85, Gender::MALE, true},
  };

  ostringstream os, os2;
  os << "Median age = 50\n"
        "Median age for females = 28\n"
        "Median age for males = 60\n"
        "Median age for employed females = 27\n"
        "Median age for unemployed females = 70\n"
        "Median age for employed males = 78\n"
        "Median age for unemployed males = 31\n";

  PrintStats(ComputeStats(persons), os2);
  ASSERT_EQUAL(os2.str(), os.str());
}

void TestBug6() {
  vector<Person> persons = {
    {31, Gender::MALE, false},
    {25, Gender::MALE, false},
    {40, Gender::FEMALE, true},
    {50, Gender::FEMALE, false},
    {58, Gender::FEMALE, true},
    {25, Gender::FEMALE, true},
    {26, Gender::FEMALE, true},
    {27, Gender::FEMALE, true},
    {28, Gender::FEMALE, true},
    {24, Gender::MALE, true},
    {20, Gender::FEMALE, true},
    {80, Gender::FEMALE, false},
    {70, Gender::FEMALE, false},
    {78, Gender::MALE, false},
    {60, Gender::MALE, false},
    {54, Gender::MALE, true},
    {21, Gender::MALE, false},
    {78, Gender::MALE, true},
    {10, Gender::FEMALE, false},
    {66, Gender::MALE, true},
    {90, Gender::MALE, true},
    {85, Gender::MALE, true},
  };

  ostringstream os, os2;
  os << "Median age = 50\n"
        "Median age for females = 28\n"
        "Median age for males = 60\n"
        "Median age for employed females = 27\n"
        "Median age for unemployed females = 70\n"
        "Median age for employed males = 78\n"
        "Median age for unemployed males = 31\n";

  PrintStats(ComputeStats(persons), os2);
  ASSERT_EQUAL(os2.str(), os.str());
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBug1);
  RUN_TEST(tr, TestBug2);
  RUN_TEST(tr, TestBug3);
  RUN_TEST(tr, TestBug4);
  RUN_TEST(tr, TestBug5);
  RUN_TEST(tr, TestBug6);
  return 0;
}

