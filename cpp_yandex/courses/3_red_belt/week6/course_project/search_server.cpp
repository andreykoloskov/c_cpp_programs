#include "search_server.h"
#include "iterator_range.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iterator>

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
  InvertedIndex new_index;

  for (string current_document; getline(document_input, current_document); ) {
    new_index.Add(current_document);
  }

  index = move(new_index);
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output)
{
    vector<int> docids(index.doc_size());
    vector<size_t> counts(index.doc_size());

    for (string current_query; getline(query_input, current_query); ) {
        iota(docids.begin(), docids.end(), 0);
        counts.assign(counts.size(), 0);
        map<int, size_t> res;

        for (const auto& word : SplitIntoWords(current_query)) {
            for (const auto& [docid, cnt] : index.Lookup(word, res)) {
                counts[docid] += cnt;
            }
        }

        size_t dist = docids.size() < 5 ? docids.size() : 5;
        partial_sort(docids.begin(), docids.begin() + dist, docids.end(),
                [&counts](int lhs, int rhs) {
                    int64_t lhs_docid = lhs;
                    auto lhs_hit_count = counts[lhs_docid];
                    int64_t rhs_docid = rhs;
                    auto rhs_hit_count = counts[rhs_docid];
                    return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
                });

        search_results_output << current_query << ':';
        for (auto docid : Head(docids, dist)) {
            if (counts[docid] > 0) {
                search_results_output << " {"
                    << "docid: " << docid << ", "
                    << "hitcount: " << counts[docid] << '}';
            }
        }
        search_results_output << endl;
    }
}

void InvertedIndex::Add(string& document)
{
    const size_t docid = docs.size();
    for (auto& word : SplitIntoWords(document)) {
        ++index[move(word)][docid];
    }
    docs.push_back(move(document));
}

const map<int, size_t>& InvertedIndex::Lookup(const string& word, const map<int, size_t>& res) const
{
    if (auto it = index.find(word); it != index.end()) {
        return it->second;
    } else {
        return res;
    }
}
