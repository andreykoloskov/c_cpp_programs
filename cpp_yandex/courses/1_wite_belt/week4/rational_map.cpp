/*
Часть 5
Наконец, вам нужно реализовать возможность использовать объекты класса Rational в качестве элементов контейнера set и ключей в контейнере map. Пример:
set<Rational> rationals;
rationals.insert(Rational(1, 2));
rationals.insert(Rational(1, 3));

map<Rational, string> name;
name[Rational(1, 2)] = "одна вторая";
*/

#include <iostream>
#include <map>
#include <set>
#include <vector>
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

bool operator<(const Rational& lhs, const Rational& rhs) {
    return lhs.Numerator() * rhs.Denominator()
            < rhs.Numerator() * lhs.Denominator();
}

int main() {
    {
        const set<Rational> rs = {{1, 2}, {1, 25}, {3, 4}, {3, 4}, {1, 2}};
        if (rs.size() != 3) {
            cout << "Wrong amount of items in the set" << endl;
            return 1;
        }

        vector<Rational> v;
        for (auto x : rs) {
            v.push_back(x);
        }
        if (v != vector<Rational>{{1, 25}, {1, 2}, {3, 4}}) {
            cout << "Rationals comparison works incorrectly" << endl;
            return 2;
        }
    }

    {
        map<Rational, int> count;
        ++count[{1, 2}];
        ++count[{1, 2}];

        ++count[{2, 3}];

        if (count.size() != 2) {
            cout << "Wrong amount of items in the map" << endl;
            return 3;
        }
    }

    cout << "OK" << endl;
    return 0;
}
