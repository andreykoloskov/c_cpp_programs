#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "test_runner.h"

using namespace std;


enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};



// Это пример функции, его не нужно отправлять вместе с функцией PrintStats
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
  if (range_begin == range_end) {
    return 0;
  }
  vector<typename InputIt::value_type> range_copy(range_begin, range_end);
  auto middle = begin(range_copy) + range_copy.size() / 2;
  nth_element(
      begin(range_copy), middle, end(range_copy),
      [](const Person& lhs, const Person& rhs) {
        return lhs.age < rhs.age;
      }
  );
  return middle->age;
}

template<typename Func>
void PrintStatsInner(vector<Person> persons, const string str, Func predicate) {
  cout << str << " = "
       << ComputeMedianAge(persons.begin(), partition(persons.begin(), persons.end(), predicate))
       << endl;
}

void PrintStats(vector<Person> persons) {
  PrintStatsInner(persons, "Median age",
      [](auto& p) { return true; });
  PrintStatsInner(persons, "Median age for females",
      [](auto& p) { return p.gender == Gender::FEMALE; });
  PrintStatsInner(persons, "Median age for males",
      [](auto& p) { return p.gender == Gender::MALE; });
  PrintStatsInner(persons, "Median age for employed females",
      [](auto& p) { return p.gender == Gender::FEMALE && p.is_employed; });
  PrintStatsInner(persons, "Median age for unemployed females",
      [](auto& p) { return p.gender == Gender::FEMALE && !p.is_employed; });
  PrintStatsInner(persons, "Median age for employed males",
      [](auto& p) { return p.gender == Gender::MALE && p.is_employed; });
  PrintStatsInner(persons, "Median age for unemployed males",
      [](auto& p) { return p.gender == Gender::MALE && !p.is_employed; });
}

int main() {
  vector<Person> persons = {
      {31, Gender::MALE, false},
      {40, Gender::FEMALE, true},
      {24, Gender::MALE, true},
      {20, Gender::FEMALE, true},
      {80, Gender::FEMALE, false},
      {78, Gender::MALE, false},
      {10, Gender::FEMALE, false},
      {55, Gender::MALE, true},
  };
  PrintStats(persons);
  return 0;
}
