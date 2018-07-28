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

istream& operator>>(istream& stream, Date& date) {
    string s;
    stream >> s;
    stringstream ss(s);
    int d, m, y;
    char c1, c2;
    ss >> y >> c1 >> m >> c2 >> d;

    if (ss.eof() && c1 == '-' && c2 == '-') {
        if (y < 0 || y > 9999)
            throw invalid_argument("Wrong date format: " + s);
        if (m < 1 || m > 12)
            throw invalid_argument("Month value is invalid: " + to_string(m));
        if (d < 1 || d > 31)
            throw invalid_argument("Day value is invalid: " + to_string(d));

        date = Date(y, m, d);
    } else {
        throw invalid_argument("Wrong date format: " + s);
    }

    return stream;
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

class Database {
public:
    void AddEvent(const Date& date, const string& event) {
        innerDB[date].insert(event);
    }

    bool DeleteEvent(const Date& date, const string& event) {
        if (innerDB.count(date) == 0)
            return false;

        if (innerDB[date].count(event) == 0)
            return false;

        innerDB[date].erase(event);

        if (innerDB[date].size() == 0)
            innerDB.erase(date);

        return true;
    }

    int  DeleteDate(const Date& date) {
        if (innerDB.count(date) == 0)
            return 0;

        int cnt = innerDB[date].size();
        innerDB.erase(date);
        return cnt;
    }

    set<string> Find(const Date& date) const {
        if (innerDB.count(date) == 0)
            return {};

        return innerDB.at(date);
    }

    void Print() const {
        for (auto d : innerDB)
           for (auto s : d.second)
               cout << d.first << " " << s << endl;
    }

private:
    map<Date, set<string>> innerDB;
};

int main() {
    Database db;

    string command;

    try {
        while (getline(cin, command)) {
            istringstream ss(command);
            string op, event;
            Date date;

            ss >> op;
            if (op == "Add") {
                try {
                    ss >> date;
                } catch (exception &ex) {
                    cout << ex.what() << endl;
                    break;
                }
                ss >> event;
                db.AddEvent(date, event);
            } else if (op == "Del") {
                try {
                    ss >> date;
                } catch (exception &ex) {
                    cout << ex.what() << endl;
                    break;
                }

                ss >> event;
                if (event.empty()) {
                    int n = db.DeleteDate(date);
                    cout << "Deleted " << n << " events" << endl;
                } else {
                    if (db.DeleteEvent(date, event))
                        cout << "Deleted successfully" << endl;
                    else
                        cout << "Event not found" << endl;
                }
            } else if (op == "Find") {
                try {
                    ss >> date;
                } catch (exception &ex) {
                    cout << ex.what() << endl;
                    break;
                }
                set<string> events = db.Find(date);
                for (auto e : events) {
                    cout << e << endl;
                }
            } else if (op == "Print") {
                db.Print();
            } else if (!op.empty()) {
                throw runtime_error("Unknown command: " + op);
            }
        }
    } catch (exception &ex) {
        cout << ex.what() << endl;
    }

    return 0;
}


