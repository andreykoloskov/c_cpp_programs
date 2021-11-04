#pragma once

#include <istream>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include <numeric>
#include <deque>

#include "inverted_index.h"

using namespace std;

class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    InvertedIndex index;
};
