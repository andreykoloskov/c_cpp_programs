#pragma once

#include <iostream>
#include <iomanip>
#include <sstream>
#include <map>
#include <set>

using namespace std;

class Date {
public:
    Date(int y = 0, int m = 0, int d = 0): year(y), month(m), day(d) {}
    int GetYear() const { return year; }
    int GetMonth() const { return month; }
    int GetDay() const { return day; }

private:
    int year;
    int month;
    int day;
};

Date ParseDate(istream& is);
ostream& operator<<(ostream& stream, const Date& date);
bool operator<(const Date& lhs, const Date& rhs);
bool operator==(const Date& lhs, const Date& rhs);
bool operator!=(const Date& lhs, const Date& rhs);
bool operator>(const Date& lhs, const Date& rhs);
bool operator<=(const Date& lhs, const Date& rhs);
bool operator>=(const Date& lhs, const Date& rhs);
