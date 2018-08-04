#include <iostream>
#include <cctype>
#include <map>

enum class Place {
    begin = 0,
    in_multiplier = 1,
    in_degree = 2,
};

class Member {
public:
    Member(): sign(1), begin_multiplier(1), multiplier(1),
            begin_degree(1), degree(0) {}

    int GetSign() { return sign; }
    int GetMultiplier() { return multiplier; }
    int GetDegree() { return degree; }

    void Negative() {
        sign = -1;
    }

    void UpdateMultiplier(char ch) {
        if (begin_multiplier) {
            multiplier = ch - '0';
            begin_multiplier = 0;
        } else {
            multiplier = multiplier * 10 + (ch - '0');
        }
    }

    void SetDegree() {
        degree = 1;
    }

    void UpdateDegree(char ch) {
        if (begin_degree) {
            degree = (ch - '0');
            begin_degree = 0;
        } else {
                degree = degree * 10 + (ch - '0');
        }
    }

    void Reset() {
        sign = 1;
        begin_multiplier = 1;
        multiplier = 1;
        begin_degree = 1;
        degree = 0;
    }

private:
    int sign, begin_multiplier, multiplier, begin_degree, degree;
};

struct Comparator {
    bool operator()( const int& lhs, const int& rhs ) const {
        return rhs < lhs;
    }
};

class Polynomial {
public:
    Polynomial() {}

    void AddMemberCoefficients(Member &member) {
        if (polynomial_coefficients.count(member.GetDegree())) {
            polynomial_coefficients[member.GetDegree()]
                    += member.GetSign() * member.GetMultiplier();
        } else {
            polynomial_coefficients[member.GetDegree()]
                    = member.GetSign() * member.GetMultiplier();
        }
        member.Reset();
    }

    std::string ReturnDerivative() {
        std::string polynomial;
        for (auto item : derivative_coefficients) {
            if (item.second < 0) {
                if (item.second == -1 && item.first > 0) {
                    polynomial += "-";
                } else {
                    polynomial += std::to_string(item.second);
                }
            } else if (item.second > 0) {
                if (polynomial.length() > 0) {
                    polynomial += "+";
                }

                if (!(item.second == 1 && item.first > 0)) {
                    polynomial += std::to_string(item.second);
                }
            } else {
                continue;
            }

            if (item.first == 1) {
                if (item.second != -1 && item.second != 1 && item.first > 0) {
                    polynomial += "*";
                }
                polynomial += "x";
            } else if (item.first > 0) {
                if (item.second != -1 && item.second != 1 && item.first > 0) {
                    polynomial += "*";
                }
                polynomial += "x^" + std::to_string(item.first);
            }
        }

        return polynomial;
    }

    void Derivative() {
        for (auto item : polynomial_coefficients) {
            if (item.first == 0 || item.second == 0) {
                continue;
            }
            derivative_coefficients[item.first - 1] = item.first * item.second;
        }
    }

private:
    std::map<int, int, Comparator> polynomial_coefficients;
    std::map<int, int, Comparator> derivative_coefficients;
};

std::string derivative(std::string polynomial_string) {
    auto place = Place::begin;
    auto member = Member();
    auto polynomial = Polynomial();

    for (auto ch : polynomial_string) {
        if (std::isspace(ch)) {
            continue;
        } else if (ch == '+') {
            polynomial.AddMemberCoefficients(member);
            place = Place::begin;
        } else if (ch == '-') {
            if (place != Place::begin) {
                polynomial.AddMemberCoefficients(member);
                place = Place::begin;
            }
            member.Negative();
        } else if (std::isdigit(ch)) {
            if (place == Place::begin) {
                place = Place::in_multiplier;
            }

            if (place == Place::in_multiplier) {
                member.UpdateMultiplier(ch);
            } else if (place == Place::in_degree) {
                member.UpdateDegree(ch);
            }
        } else if (ch == 'x') {
            place = Place::in_degree;
            member.SetDegree();
        }
    }

    if (polynomial_string.size() > 0) {
        polynomial.AddMemberCoefficients(member);
    }

    polynomial.Derivative();
    return polynomial.ReturnDerivative();
}

int main() {
    std::string polynomial_string;
    std::getline(std::cin, polynomial_string);

    std::cout << derivative(polynomial_string) << std::endl;

    return 0;
}