#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template <class T>
ostream& operator << (ostream& os, const vector<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class T>
ostream& operator << (ostream& os, const set<T>& s) {
  os << "{";
  bool first = true;
  for (const auto& x : s) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << x;
  }
  return os << "}";
}

template <class K, class V>
ostream& operator << (ostream& os, const map<K, V>& m) {
  os << "{";
  bool first = true;
  for (const auto& kv : m) {
    if (!first) {
      os << ", ";
    }
    first = false;
    os << kv.first << ": " << kv.second;
  }
  return os << "}";
}

template<class T, class U>
void AssertEqual(const T& t, const U& u, int line = 0) {
  if (t != u) {
    ostringstream os;
    os << "Assertion failed: " << t << " != " << u;
    os << " line: " << line;
    throw runtime_error(os.str());
  }
}

void Assert(bool b, int line) {
  AssertEqual(b, true, line);
}

class TestRunner {
public:
  template <class TestFunc>
  void RunTest(TestFunc func, const string& test_name) {
    try {
      func();
      cerr << test_name << " OK" << endl;
    } catch (exception& e) {
      ++fail_count;
      cerr << test_name << " fail: " << e.what() << endl;
    } catch (...) {
      ++fail_count;
      cerr << "Unknown exception caught" << endl;
    }
  }

  ~TestRunner() {
    if (fail_count > 0) {
      cerr << fail_count << " unit tests failed. Terminate" << endl;
      exit(1);
    }
  }

private:
  int fail_count = 0;
};

int GetDistinctRealRootCount(double a, double b, double c) {
    if (a == 0) {
      if (b != 0)
        return 1;
      else
        return 0;
    } else {
      double d = b * b - 4 * a * c;
      if (d > 0)
        return 2;
      else if (d == 0)
        return 1;
      else
        return 0;
    }
}

#define AssertEqual2(t, u) AssertEqual(t, u, __LINE__)
#define Assert2(t) Assert(t, __LINE__)
#define RunTest2(func) RunTest(func, #func)

string FindNameByYear(const map<int, string>& names, int year) {
  string name;  // изначально имя неизвестно

  // перебираем всю историю по возрастанию ключа словаря, то есть в хронологическом порядке
  for (const auto& item : names) {
    // если очередной год не больше данного, обновляем имя
    if (item.first <= year) {
      name = item.second;
    } else {
      // иначе пора остановиться, так как эта запись и все последующие относятся к будущему
      break;
    }
  }

  return name;
}

class Person {
public:
    void ChangeFirstName(int year, const string& first_name) {
      first_names[year] = first_name;
    }
    void ChangeLastName(int year, const string& last_name) {
      last_names[year] = last_name;
    }
    string GetFullName(int year) {
      // получаем имя и фамилию по состоянию на год year
      const string first_name = FindNameByYear(first_names, year);
      const string last_name = FindNameByYear(last_names, year);

      // если и имя, и фамилия неизвестны
      if (first_name.empty() && last_name.empty()) {
        return "Incognito";

        // если неизвестно только имя
      } else if (first_name.empty()) {
        return last_name + " with unknown first name";

        // если неизвестна только фамилия
      } else if (last_name.empty()) {
        return first_name + " with unknown last name";

        // если известны и имя, и фамилия
      } else {
        return first_name + " " + last_name;
      }
    }
private:
    map<int, string> first_names;
    map<int, string> last_names;
};

void TestIncognito() {
    Person person;
    AssertEqual2(person.GetFullName(2000), "Incognito");
}

void TestUnknownFirstName() {
    Person person;
    person.ChangeLastName(2000, "Sergeeva");
    AssertEqual2(person.GetFullName(2000), "Sergeeva with unknown first name");
}

void TestUnknownLastName() {
    Person person;
    person.ChangeFirstName(2000, "Polina");
    AssertEqual2(person.GetFullName(2000), "Polina with unknown last name");
}

void TestFirstAndLastName() {
    Person person;
    person.ChangeFirstName(2000, "Polina");
    person.ChangeLastName(2000, "Sergeeva");
    AssertEqual2(person.GetFullName(2000), "Polina Sergeeva");
}

int main() {
  TestRunner runner;

  runner.RunTest2(TestIncognito);
  runner.RunTest2(TestUnknownFirstName);
  runner.RunTest2(TestUnknownLastName);
  runner.RunTest2(TestFirstAndLastName);

  return 0;
}
