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

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
  for (string current_query; getline(query_input, current_query); ) {
    map<size_t, size_t> docid_count;
    map<size_t, size_t> res;
    for (const auto& word : SplitIntoWords(current_query)) {
      for (const auto& [docid, cnt] : index.Lookup(word, res)) {
        docid_count[docid] += cnt;
      }
    }

    vector<pair<size_t, size_t>> search_results(
      make_move_iterator(docid_count.begin()), make_move_iterator(docid_count.end())
    );
    sort(
      begin(search_results),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );

    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
        if (docid >= 0) {
            search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << hitcount << '}';
        }
    }
    search_results_output << endl;
  }
}

void InvertedIndex::Add(string& document) {
  const size_t docid = docs.size();
  for (auto& word : SplitIntoWords(document)) {
    ++index[move(word)][docid];
  }
  docs.push_back(move(document));
}

const map<size_t, size_t>& InvertedIndex::Lookup(const string& word, const map<size_t, size_t>& res) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return res;
  }
}
