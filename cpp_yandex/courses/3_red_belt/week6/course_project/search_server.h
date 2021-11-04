#pragma once

#include <iostream>
#include <istream>
#include <ostream>
#include <vector>
#include <map>
#include <string>
#include <numeric>
#include <iterator>
#include <deque>
#include <mutex>
#include <future>
#include <functional>
#include <array>
#include "inverted_index.h"

using namespace std;


class SearchServer {
public:
    SearchServer() = default;
    explicit SearchServer(istream& document_input);
    void UpdateDocumentBase(istream& document_input);
    void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
    struct Element
    {
        string current_query;
        size_t quantity;
        array<int, 5> docids;
        array<size_t, 5> hitcounts;
    };

    const static size_t max_size = 50000;
    const static size_t query_max_size = 500000;
    InvertedIndex index;
    mutex m;
};
