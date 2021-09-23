/*
В этой задаче вам нужно разработать набор юнит-тестов для функции
bool IsPalindrom(const string& s);

Эта функция проверяет, является ли строка s палиндромом. Палиндром — это слово или фраза, которые одинаково читаются слева направо и справа налево. Примеры палиндромов: madam, level, wasitacaroracatisaw

Разработайте набор юнит-тестов, который будет принимать правильные реализации функции IsPalindrom и отвергать неправильные. При этом учитывайте, что правильная реализация функции:

считает пустую строку палиндромом;

считает строку из одного символа палиндромом;

осуществляет обычное сравнение символов на равенство, не игнорируя никакие символы, в том числе пробельные.

При разработке тестов подумайте, какие ошибки можно допустить при реализации функции IsPalindrom. Примеры ошибок:

игнорируется первый или последний символ;

сравнение соответствующих символов завершается не в середине строки, а раньше;

игнорируются пробелы

Начать работу вы можете с шаблона, который содержит наш фреймворк юнит-тестов и заготовку функции IsPalindrom.
*/

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

bool IsPalindrom(const string& str) {
  for (size_t i = 0; i < str.size() / 2; ++i) {
    if (str[i] != str[str.size() - i - 1]) {
      return false;
    }
  }
  return true;
}

void TestIsPolindromEmpty() {
  Assert2(IsPalindrom(""));
}

void TestIsPolindromOneSymbol() {
  Assert2(IsPalindrom("a"));
  Assert2(IsPalindrom(" "));
}

void TestIsPolindromMenySymbol() {
  Assert2(IsPalindrom("abba"));
  Assert2(IsPalindrom("abmba"));
  Assert2(IsPalindrom("ab  ba"));
  Assert2(IsPalindrom("ab ba"));
}

void TestIsNotPolindrom() {
  Assert2(!IsPalindrom("abma"));
  Assert2(!IsPalindrom("abcma"));
  Assert2(!IsPalindrom("abbb"));
  Assert2(!IsPalindrom("abcbb"));
  Assert2(!IsPalindrom(" abba  "));
  Assert2(!IsPalindrom("  abba "));
}

int main() {
  TestRunner runner;

  runner.RunTest2(TestIsPolindromEmpty);
  runner.RunTest2(TestIsPolindromOneSymbol);
  runner.RunTest2(TestIsPolindromMenySymbol);
  runner.RunTest2(TestIsNotPolindrom);

  return 0;
}
