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

int main() {
    {
        Rational r1(4, 6);
        Rational r2(2, 3);
        bool equal = r1 == r2;
        if (!equal) {
            cout << "4/6 != 2/3" << endl;
            return 1;
        }
    }

    {
        Rational a(2, 3);
        Rational b(4, 3);
        Rational c = a + b;
        bool equal = c == Rational(2, 1);
        if (!equal) {
            cout << "2/3 + 4/3 != 2" << endl;
            return 2;
        }
    }

    {
        Rational a(5, 7);
        Rational b(2, 9);
        Rational c = a - b;
        bool equal = c == Rational(31, 63);
        if (!equal) {
            cout << "5/7 - 2/9 != 31/63" << endl;
            return 3;
        }
    }

    cout << "OK" << endl;
    return 0;
}
