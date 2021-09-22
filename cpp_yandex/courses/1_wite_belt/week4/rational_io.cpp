/*
В этой части вам нужно реализовать операторы ввода и вывода для класса Rational. В результате у вас должен работать, например, такой код
Rational r;
cin >> r;
cout << r;
*/

#include <iostream>
#include <sstream>
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

ostream& operator<<(ostream& stream, const Rational& rational) {
    stream << rational.Numerator() << "/" << rational.Denominator();
    return stream;
}

istream& operator>>(istream& stream, Rational& rational) {
    int p, q;
    if (stream) {
        stream >> p;
        stream.ignore(1);
        if (stream) {
            stream >> q;
            rational = Rational(p, q);
        }
    }
    return stream;
}

int main() {
    {
        ostringstream output;
        output << Rational(-6, 8);
        if (output.str() != "-3/4") {
            cout << "Rational(-6, 8) should be written as \"-3/4\"" << endl;
            return 1;
        }
    }

    {
        istringstream input("5/7");
        Rational r;
        input >> r;
        bool equal = r == Rational(5, 7);
        if (!equal) {
            cout << "5/7 is incorrectly read as " << r << endl;
            return 2;
        }
    }

    {
        istringstream input("5/7 10/8");
        Rational r1, r2;
        input >> r1 >> r2;
        bool correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Multiple values are read incorrectly: " << r1 << " " << r2 << endl;
            return 3;
        }

        input >> r1;
        input >> r2;
        correct = r1 == Rational(5, 7) && r2 == Rational(5, 4);
        if (!correct) {
            cout << "Read from empty stream shouldn't change arguments: " << r1 << " " << r2 << endl;
            return 4;
        }
    }

    cout << "OK" << endl;
    return 0;
}
