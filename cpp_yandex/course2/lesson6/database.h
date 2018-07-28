#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <functional>
#include <tuple>
#include <algorithm>

#include "date.h"

using namespace std;

struct DateEvent {
    Date date;
    string event;
};

ostream& operator<<(ostream& os, const DateEvent& de);

class Database {
public:
    void
    Add(const Date& date, const string& event);

    int
    RemoveIf(function<bool(const Date& date, const string& event)> Evaluate);

    vector<DateEvent>
    FindIf(function<bool(const Date& date, const string& event)> Evaluate);

    DateEvent
    Last(const Date& date);

    void
    Print(ostream& os) const;

private:
    map<Date, pair<set<string>, vector<string>>> innerDB;
};
