/*
Функция
int GetDistinctRealRootCount(double a, double b, double c);

возвращает количество уникальных действительных корней уравнения ax² + bx + c = 0. Разработайте набор юнит-тестов для проверки корректности реализации этой функции. Случай, когда все три коэффициента равны нулю, тестировать не надо.

Начать работу вы можете с шаблона, который содержит наш фреймворк юнит-тест и заготовку функции GetDistinctRealRootCount.
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

void Test1() {
  AssertEqual2(GetDistinctRealRootCount(0, 1, 1), 1);
}

void Test2() {
  AssertEqual2(GetDistinctRealRootCount(0, 0, 1), 0);
}

void Test3() {
  AssertEqual2(GetDistinctRealRootCount(1, 1, 1), 0);
}

void Test4() {
  AssertEqual2(GetDistinctRealRootCount(1, 2, 1), 1);
}

void Test5() {
  AssertEqual2(GetDistinctRealRootCount(1, 5, 1), 2);
}

int main() {
  TestRunner runner;

  runner.RunTest2(Test1);
  runner.RunTest2(Test2);
  runner.RunTest2(Test3);
  runner.RunTest2(Test4);
  runner.RunTest2(Test5);

  return 0;
}
