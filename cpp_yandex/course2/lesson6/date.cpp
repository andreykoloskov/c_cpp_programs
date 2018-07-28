#include "date.h"

using namespace std;

Date
ParseDate(istream& is) {
    Date date;
    string s;
    is >> s;
    stringstream ss(s);
    int d, m, y;
    char c1, c2;
    ss >> y >> c1 >> m >> c2 >> d;

    if (ss.eof() && c1 == '-' && c2 == '-') {
        if (y < 0 || y > 9999)
            throw invalid_argument("Wrong date format: " + s);
        if (m < 1 || m > 12)
            throw invalid_argument(
                    "Month value is invalid: " + to_string(m));
        if (d < 1 || d > 31)
            throw invalid_argument("Day value is invalid: " + to_string(d));

        date = Date(y, m, d);
    } else {
        throw invalid_argument("Wrong date format: " + s);
    }

    return date;
}

ostream& operator<<(ostream& stream, const Date& date) {
    return stream << setfill('0') << setw(4) << date.GetYear()
                  << "-" << setw(2) << date.GetMonth()
                  << "-" << setw(2) << date.GetDay();
}

bool operator<(const Date& lhs, const Date& rhs) {
    return (lhs.GetYear() < rhs.GetYear())
           || (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() < rhs.GetMonth())
           || (lhs.GetYear() == rhs.GetYear() && lhs.GetMonth() == rhs.GetMonth()
               && lhs.GetDay() < rhs.GetDay());
}

bool operator==(const Date& lhs, const Date& rhs) {
    return lhs.GetYear() == rhs.GetYear()
            && lhs.GetMonth() == rhs.GetMonth()
            && lhs.GetDay() == rhs.GetDay();
}

bool operator!=(const Date& lhs, const Date& rhs) {
    return !(lhs == rhs);
}

bool operator>(const Date& lhs, const Date& rhs) {
    return !(lhs < rhs || lhs == rhs);
}

bool operator<=(const Date& lhs, const Date& rhs) {
    return lhs < rhs || lhs == rhs;
}

bool operator>=(const Date& lhs, const Date& rhs) {
    return lhs > rhs || lhs == rhs;
}
