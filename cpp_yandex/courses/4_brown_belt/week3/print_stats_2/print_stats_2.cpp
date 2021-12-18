#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <optional>
#include "test_runner.h"

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

vector<Person> ReadPeople(istream& input) {
  int count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }

  return result;
}

void AgeProcess(const vector<Person>& people_age) {
  int adult_age;
  cin >> adult_age;

  auto adult_begin = lower_bound(
    begin(people_age), end(people_age), adult_age, [](const Person& lhs, int age) {
      return lhs.age < age;
    }
  );

  cout << "There are " << std::distance(adult_begin, end(people_age))
       << " adult people for maturity age " << adult_age << '\n';
}

void WealthyProcess(const vector<Person>& people_wealthy) {
  int count;
  cin >> count;
  auto head = Head(people_wealthy, count);

  int total_income = accumulate(
    head.begin(), head.end(), 0, [](int cur, const Person& p) {
      return p.income + cur;
    }
  );

  cout << "Top-" << count << " people have total income " << total_income << '\n';
}

optional<string> PopularNameProcess(const vector<Person>& people_name, char gender) {
  auto male = find_if(begin(people_name), end(people_name), [](const Person& p) {
    return p.is_male;
  });

  auto start = people_name.begin();
  auto finish = people_name.end();

  if (gender == 'M') {
    start = male;
  }
  else {
    finish = male;
  }

  if (start == finish) {
    return nullopt;
  } else {
    const string* most_popular_name = &start->name;
    int count = 1;
    for (auto i = start; i != finish; ) {
      auto same_name_end = find_if_not(i, finish, [i](const Person& p) {
        return p.name == i->name;
      });
      auto cur_name_count = std::distance(i, same_name_end);
      if (cur_name_count > count) {
        count = cur_name_count;
        most_popular_name = &i->name;
      }
      i = same_name_end;
    }
    return *most_popular_name;
  }
}

int main() {
  vector<Person> people_age = ReadPeople(cin);
  sort(begin(people_age), end(people_age), [](const Person& lhs, const Person& rhs) {
    return lhs.age < rhs.age;
  });

  vector<Person> people_wealthy = people_age;
  sort(begin(people_wealthy), end(people_wealthy), [](const Person& lhs, const Person& rhs) {
    return lhs.income > rhs.income;
  });

  vector<Person> people_name = people_age;
  sort(begin(people_name), end(people_name), [](const Person& lhs, const Person& rhs) {
    return lhs.is_male < rhs.is_male || (lhs.is_male == rhs.is_male && lhs.name < rhs.name);
  });
  auto man_name = PopularNameProcess(people_name, 'M');
  auto woman_name = PopularNameProcess(people_name, 'W');

  for (string command; cin >> command; ) {
    if (command == "AGE") {
      AgeProcess(people_age);
    } else if (command == "WEALTHY") {
      WealthyProcess(people_wealthy);
    } else if (command == "POPULAR_NAME") {
      char gender;
      cin >> gender;
      optional<string> name = (gender == 'M') ? man_name : woman_name;

      if (!name) {
        cout << "No people of gender " << gender << '\n';
      }
      else {
        cout << "Most popular name among people of gender " << gender << " is "
             << name.value() << '\n';
      }
    }
  }
}