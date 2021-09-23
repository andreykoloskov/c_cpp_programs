/*
����� Rational ������������ ����� ������������ ����� � ����� ��������� ���������
class Rational {
public:
  Rational();
  Rational(int numerator, int denominator);

  int Numerator() const;
  int Denominator() const;
};

������ ����������, ������������� � ���������� ���������� ������ Rational:

����������� �� ��������� ������ ��������� ����� � ���������� 0 � ������������ 1.

��� ��������������� ������� ������ Rational � ����������� p � q ������ ����������� ���������� ����� p/q.

���� ����� p/q �������������, �� ������ Rational(p, q) ������ ����� ������������� ��������� � ������������� �����������.

���� ����� p/q �������������, �� ������ Rational(p, q) ������ ����� ������������� ��������� � ����������� (�������� �������� �� ������ Rational(-2, -3)).

���� ��������� ����� ����� ����, �� ����������� ������ ���� ����� 1.

  ������������ ����� ����-������, ������� ����� ��������� ������������ ���������� ������ Rational. ����������� ������, ����� ����������� ����� ����, �� ����.

������ ������ �� ������ � �������, ������� �������� ��� ��������� ����-������ � ��������� ������ Rational.

����������
����� ��������� ������ ����� ���������, ��� �� �� �������� ������ Rational. � ��������� ������ �� �������� ������ ����������. ���� ����������� ������� ����� ������������ ������� � ������ (�������, ��������, � ������ "����� ��� ������� IsPalindrom" ������������� ������� ������� IsPalindrom), �� �� ����� ��� ������ ��� �������.
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

int GreatestCommonDivisor(int a, int b) {
  if (b == 0) {
    return a;
  } else {
    return GreatestCommonDivisor(b, a % b);
  }
}

class Rational {
public:
    Rational() {
      numerator = 0;
      denominator = 1;
    }
    Rational(int new_numerator, int new_denominator) {
      const int gcd = GreatestCommonDivisor(new_numerator, new_denominator);
      numerator = new_numerator / gcd;
      denominator = new_denominator / gcd;
      if (denominator < 0) {
        denominator = -denominator;
        numerator = -numerator;
      }
    }

    int Numerator() const {
      return numerator;
    }

    int Denominator() const {
      return denominator;
    }

private:
    int numerator;
    int denominator;
};

#define TestRationalEqual(r1, n, d) TestRationalEqual2(r1, n, d, __LINE__)

void TestRationalEqual2(Rational r1, int numerator, int denominator, int line) {
  AssertEqual(r1.Numerator(), numerator, line);
  AssertEqual(r1.Denominator(), denominator, line);
}

void TestRationalByDefault() {
    TestRationalEqual(Rational(), 0, 1);
}

void TestFractionReduction() {
  TestRationalEqual(Rational(4, 6), 2, 3);
  TestRationalEqual(Rational(-4, 6), -2, 3);
  TestRationalEqual(Rational(4, -6), -2, 3);
  TestRationalEqual(Rational(-4, -6), 2, 3);
}

void TestFractionNegative() {
  TestRationalEqual(Rational(2, -3), -2, 3);
  TestRationalEqual(Rational(0, -3), 0, 1);
}

void TestFractionPositive() {
  TestRationalEqual(Rational(-2, -3), 2, 3);
}

void TestNumeratorIsNull() {
  TestRationalEqual(Rational(0, 2), 0, 1);
}

int main() {
  TestRunner runner;

  runner.RunTest2(TestRationalByDefault);
  runner.RunTest2(TestFractionReduction);
  runner.RunTest2(TestFractionNegative);
  runner.RunTest2(TestFractionPositive);
  runner.RunTest2(TestNumeratorIsNull);

  return 0;
}
