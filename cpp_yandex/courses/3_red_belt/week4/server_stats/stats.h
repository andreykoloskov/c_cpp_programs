#pragma once

#include "http_request.h"

#include <string_view>
#include <map>
#include <set>
#include <string>

using namespace std;

class Stats {
public:
    Stats();
    void AddMethod(string_view method);
    void AddUri(string_view uri);
    const map<string_view, int>& GetMethodStats() const;
    const map<string_view, int>& GetUriStats() const;
private:
    set<string> m_;
    set<string> u_;
    map<string_view, int> methods_;
    map<string_view, int> uris_;
};

HttpRequest ParseRequest(string_view line);
string_view ParseRequestInner(string_view& line);