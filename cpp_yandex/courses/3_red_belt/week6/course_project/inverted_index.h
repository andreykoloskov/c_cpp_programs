#pragma once

#include <vector>
#include <map>
#include <string>
#include <numeric>
#include <deque>

using namespace std;

class InvertedIndex {
public:
    void Add(string& document);
    const vector<pair<size_t, size_t>>& Lookup(string_view word, const vector<pair<size_t, size_t>>& res) const;
    const string& GetDocument(size_t id) const { return docs[id]; }
    size_t doc_size() { return docs.size(); }

private:
    map<string_view, vector<pair<size_t, size_t>>> index;
    deque<string> docs;
};

