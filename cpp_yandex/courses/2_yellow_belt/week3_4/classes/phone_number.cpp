/*
Вам дан заголовочный файл phone_number.h, содержащий объявление класса PhoneNumber. При этом в комментариях описано поведение, которое ожидается от реализации этого класса.

#pragma once

#include <string>

using namespace std;

class PhoneNumber {
public:
  Принимает строку в формате +XXX-YYY-ZZZZZZ
     Часть от '+' до первого '-' - это код страны.
     Часть между первым и вторым символами '-' - код города
     Всё, что идёт после второго символа '-' - местный номер.
     Код страны, код города и местный номер не должны быть пустыми.
     Если строка не соответствует этому формату, нужно выбросить исключение invalid_argument. Проверять, что номер содержит только цифры, не нужно.

     Примеры:
     * +7-495-111-22-33
     * +7-495-1112233
     * +323-22-460002
     * +1-2-coursera-cpp
     * 1-2-333 - некорректный номер - не начинается на '+'
     * +7-1233 - некорректный номер - есть только код страны и города
explicit PhoneNumber(const string& international_number);

string GetCountryCode() const;
string GetCityCode() const;
string GetLocalNumber() const;
string GetInternationalNumber() const;

private:
    string country_code_;
    string city_code_;
    string local_number_;
};
*/

#include <algorithm>
#include <stdexcept>
#include <sstream>
#include "phone_number.h"

PhoneNumber::PhoneNumber(const string &international_number) {
    istringstream is(international_number);

    char sign = is.get();
    std::getline(is, country_code_, '-');
    std::getline(is, city_code_, '-');
    std::getline(is, local_number_);

    if (sign != '+' || country_code_.empty() || city_code_.empty() || local_number_.empty()) {
        throw invalid_argument("Phone number must begin with '+' symbol and contain 3 parts separated by '-' symbol: " + international_number);
    }
}

string PhoneNumber::GetCountryCode() const {
    return PhoneNumber::country_code_;
}

string PhoneNumber::GetCityCode() const {
    return PhoneNumber::city_code_;
}

string PhoneNumber::GetLocalNumber() const {
    return PhoneNumber::local_number_;
}

string PhoneNumber::GetInternationalNumber() const {
    return "+" + PhoneNumber::country_code_ + "-" + PhoneNumber::city_code_
            + "-" + PhoneNumber::local_number_;
}

#include <iostream>

int main() {
    string s = "+7-495-111-22-33";
    PhoneNumber ph = PhoneNumber(s);
    std::cout << ph.GetCountryCode() << " " << ph.GetCityCode() << " "
            << ph.GetLocalNumber() << " " << ph.GetInternationalNumber()
            << std::endl;
}