#include "inverted_index.h"
#include "utilities.h"

void InvertedIndex::Add(string& document)
{
    docs.push_back(move(document));
    const size_t docid = docs.size() - 1;
    for (string_view word : SplitIntoWords(docs.back())) {
        auto& d = index[word];
        if (d.size() != 0 && d.back().first == docid) {
            ++(d.back().second);
        }
        else {
            d.push_back({ docid, 1 });
        }
    }
}

const vector<pair<size_t, size_t>>& InvertedIndex::Lookup(string_view word, const vector<pair<size_t, size_t>>& res) const
{
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return res;
    }
}
