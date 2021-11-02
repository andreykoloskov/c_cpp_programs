#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

class InvertedIndex {
public:
  void Add(string& document);
  const map<size_t, size_t>& Lookup(const string& word, const map<size_t, size_t>& res) const;

  const string& GetDocument(size_t id) const {
    return docs[id];
  }

  size_t doc_size() { return docs.size(); }

private:
  map<string, map<size_t, size_t>> index;
  vector<string> docs;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};