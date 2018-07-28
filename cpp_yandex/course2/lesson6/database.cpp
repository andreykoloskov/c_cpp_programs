#include "database.h"
#include "node.h"

using namespace std;

ostream& operator<<(ostream& os, const DateEvent& de) {
    os << de.date << " " << de.event;
    return os;
}

void
Database::Add(const Date& date, const string& event) {
    if (innerDB[date].first.find(event) == innerDB[date].first.end()) {
        innerDB[date].first.insert(event);
        innerDB[date].second.push_back(event);
    }
}

int
Database::RemoveIf(
        function<bool(const Date& date, const string& event)> Evaluate) {
    int cnt = 0;
    vector<DateEvent> de;
    for (auto& d : innerDB) {
        for (auto& e : d.second.first) {
            if (Evaluate(d.first, e)) {
                de.push_back({d.first, e});
            }
        }
    }

    for (auto& del : de) {
        innerDB[del.date].first.erase(del.event);
        innerDB[del.date].second.erase(find(innerDB[del.date].second.begin(),
                                            innerDB[del.date].second.end(),
                                            del.event));
        if (innerDB[del.date].first.size() == 0) {
            innerDB.erase(del.date);
        }
        cnt++;
    }

    return cnt;
}

vector<DateEvent>
Database::FindIf(
        function<bool(const Date& date, const string& event)> Evaluate) {
    vector<DateEvent> de;
    for (auto& d : innerDB) {
        for (auto& e : d.second.second) {
            if (Evaluate(d.first, e)) {
                de.push_back({d.first, e});
            }
        }
    }
    return de;
}

DateEvent
Database::Last(const Date& date) {
    auto it = innerDB.upper_bound(date);
    if (innerDB.size() == 0 || it == innerDB.begin()) {
        throw invalid_argument("");
    }

    --it;
    return {(*it).first, (*it).second.second.back()};
}

void
Database::Print(ostream& os) const {
    for (auto d : innerDB)
        for (auto s : d.second.second)
            os << d.first << " " << s << endl;
}
