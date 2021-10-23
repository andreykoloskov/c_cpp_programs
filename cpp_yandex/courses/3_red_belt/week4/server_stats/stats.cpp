#include "stats.h"

Stats::Stats()
    : m_({ "GET", "PUT", "POST", "DELETE", "UNKNOWN" })
    , u_({ "/", "/order", "/product", "/basket", "/help", "unknown" })
{
    for (auto& item : m_) {
        methods_[item] = 0;
    }
    for (auto& item : u_) {
        uris_[item] = 0;
    }
}

void Stats::AddMethod(string_view method)
{
    auto it = m_.find(string(method));
    if (it == m_.end()) {
        it = m_.find("UNKNOWN");
    }
    ++methods_[*it];
}

void Stats::AddUri(string_view uri)
{
    auto it = u_.find(string(uri));
    if (it == u_.end()) {
        it = u_.find("unknown");
    }
    ++uris_[*it];
}

const map<string_view, int>& Stats::GetMethodStats() const
{
    return methods_;
}

const map<string_view, int>& Stats::GetUriStats() const
{
    return uris_;
}

HttpRequest ParseRequest(string_view line)
{
    auto method = ParseRequestInner(line);
    auto uri = ParseRequestInner(line);
    auto protocol = ParseRequestInner(line);
    return { method, uri, protocol };
}

string_view ParseRequestInner(string_view& line)
{
    size_t space = line.find(' ');
    while (space == 0) {
        line.remove_prefix(space + 1);
        space = line.find(' ');
    }
    auto result = line.substr(0, space);
    if (space != line.npos) {
        line.remove_prefix(space + 1);
    }
    return result;
}
