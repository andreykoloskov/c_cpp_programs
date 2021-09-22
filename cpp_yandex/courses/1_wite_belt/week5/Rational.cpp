/*
¬спомним класс Rational, который мы реализовали в задачах, посв€щЄнных перегрузке операторов. “ам специально говорилось, что в тестах объекты класса Rational никогда не создаютс€ с нулевым знаменателем и никогда не выполн€етс€ деление на ноль. Ќастало врем€ избавитьс€ от этого ограничени€ и научитьс€ обрабатывать нулевой знаменатель и деление на ноль. ¬ этой задаче вам предстоит это сделать.

ѕомен€йте реализацию конструктора класса Rational так, чтобы он выбрасывал исключение invalid_argument, если знаменатель  равен нулю.  роме того, переделайте реализацию оператора делени€ дл€ класса Rational так, чтобы он выбрасывал исключение domain_error, если делитель равен нулю.

Ќапомним, что вам надо реализовать интерфейс
class Rational {
public:
  Rational();
  Rational(int numerator, int denominator);

  int Numerator() const;
  int Denominator() const;
};

и оператор делени€ дл€ класса Rational
Rational operator / (const Rational& lhs, const Rational& rhs);

“ак же, как и в задачах на перегрузку операторов, вам будет предложен файл с заготовкой кода. ‘ункци€ main в этом файле содержит р€д юнит-тестов, которые позвол€т вам проверить свою реализацию. ѕри проверке вашего решени€ оно будет тестироватьс€ на более полном наборе тестов, так что успешное прохождение всех юнит-тестов не гарантирует, что ваша реализаци€ будет зачтена.
*/

#include <iostream>
#include <exception>
using namespace std;

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
        if (new_denominator == 0) {
            throw invalid_argument("delete 0 exception");
        }
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

Rational operator * (const Rational& lhs, const Rational& rhs) {
    return {
            lhs.Numerator() * rhs.Numerator(),
            lhs.Denominator() * rhs.Denominator()
    };
}

Rational operator / (const Rational& lhs, const Rational& rhs) {
    if (rhs.Numerator() == 0) {
        throw domain_error("delete 0 exception");
    }
    return lhs * Rational(rhs.Denominator(), rhs.Numerator());
}

int main() {
    try {
        Rational r(1, 0);
        cout << "Doesn't throw in case of zero denominator" << endl;
        return 1;
    } catch (invalid_argument&) {
    }

    try {
        auto x = Rational(1, 2) / Rational(0, 1);
        cout << "Doesn't throw in case of division by zero" << endl;
        return 2;
    } catch (domain_error&) {
    }

    cout << "OK" << endl;
    return 0;
}