/*
Аналогично предыдущей части, реализуйте операторы * и /. Например, следующий код должен быть валидным:
const Rational r = Rational(1, 2) * Rational(1, 3) / Rational(1, 4);
if (r == Rational(2, 3)) {
  cout << "equal";
}

Как и в предыдущем случае, гарантируется, что во всех тестах, на которых будет тестироваться ваша реализация, числители и знаменатели ВСЕХ дробей (как исходных, так и получившихся в результате соответствующих арифметических операций) будут укладываться в диапазон типа int.

Кроме того, гарантируется, что ни в одном из тестов, на которых будет тестироваться ваша реализация, не будет выполняться деление на ноль.
*/

#include <iostream>
using namespace std;

int Nod(int a, int b) {
    while (a > 0 && b > 0) {
        if (a > b) {
            a %= b;
        } else {
            b %= a;
        }
    }
    return a + b;
}

class Rational {
public:
    Rational() {
        p = 0;
        q = 1;
    }

    Rational(int numerator, int denominator) {
        if (numerator == 0) {
            p = 0;
            q = 1;
            return;
        }

        int sign = ((numerator >= 0 && denominator >= 0)
                || (numerator < 0 && denominator < 0)) ? 1 : -1;
        numerator  = (numerator >= 0) ? numerator : -numerator;
        denominator = (denominator >= 0) ? denominator : -denominator;
        int nod = Nod(numerator, denominator);

        p = sign * numerator / nod;
        q = denominator / nod;
    }

    int Numerator() const {
        return p;
    }

    int Denominator() const {
        return q;
    }

private:
    int p;
    int q;
};

bool operator==(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() == rhs.Numerator()
            && lhs.Denominator() == rhs.Denominator();
}

Rational operator+(const Rational& lhs, const Rational& rhs) {
    int nod = Nod(lhs.Denominator(), rhs.Denominator());
    int d1 = lhs.Denominator() / nod;
    int d2 = rhs.Denominator() / nod;

    return Rational(lhs.Numerator() * d2 + rhs.Numerator() * d1,
            d1 * d2 * nod);
}

Rational operator-(const Rational& lhs, const Rational& rhs) {
    int nod = Nod(lhs.Denominator(), rhs.Denominator());
    int d1 = lhs.Denominator() / nod;
    int d2 = rhs.Denominator() / nod;

    return Rational(lhs.Numerator() * d2 - rhs.Numerator() * d1,
            d1 * d2 * nod);
}

Rational operator*(const Rational& lhs, const Rational& rhs) {
    int num1 = (lhs.Numerator() >= 0) ? lhs.Numerator() : -lhs.Numerator();
    int num2 = (rhs.Numerator() >= 0) ? rhs.Numerator() : -rhs.Numerator();
    int nod1 = Nod(num1, rhs.Denominator());
    int nod2 = Nod(num2, lhs.Denominator());

    int a = lhs.Numerator() / nod1;
    int d = rhs.Denominator() / nod1;
    int b = lhs.Denominator() / nod2;
    int c = rhs.Numerator() / nod2;

    return Rational(a * c, b * d);
}

Rational operator/(const Rational& lhs, const Rational& rhs) {
    int num1 = (lhs.Numerator() >= 0) ? lhs.Numerator() : -lhs.Numerator();
    int num2 = (rhs.Numerator() >= 0) ? rhs.Numerator() : -rhs.Numerator();
    int nod1 = Nod(num1, num2);
    int nod2 = Nod(rhs.Denominator(), lhs.Denominator());

    int a = lhs.Numerator() / nod1;
    int c = rhs.Numerator() / nod1;
    int b = lhs.Denominator() / nod2;
    int d = rhs.Denominator() / nod2;

    return Rational(a * d, b * c);
}

int main() {
    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a * b;
        bool equal = c == Rational(8, 9);
        if (!equal) {
            cout << "2/3 * 4/3 != 8/9" << endl;
            return 1;
        }
    }

    {
        Rational a(5, 4);
        Rational b(15, 8);
        Rational c = a / b;
        bool equal = c == Rational(2, 3);
        if (!equal) {
            cout << "5/4 / 15/8 != 2/3" << endl;
            return 2;
        }
    }

    cout << "OK" << endl;
    return 0;
}
