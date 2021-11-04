#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>
#include <iterator>
#include <thread>

#include "search_server.h"
#include "iterator_range.h"
#include "utilities.h"

void UpdateDocumentBaseInner(istream& document_input, InvertedIndex& index, mutex& m)
{
    InvertedIndex result;
    for (string current_document; getline(document_input, current_document); ) {
        result.Add(current_document);
    }
    {
        lock_guard g(m);
        index = move(result);
    }
}

void AddQueriesStreamInner(istream& query_input, ostream& search_results_output, InvertedIndex& index, mutex& m)
{

    this_thread::sleep_for(10ms);
    vector<int> docids;
    vector<size_t> counts;

    for (string current_query; getline(query_input, current_query); ) {
        const auto words = SplitIntoWords(current_query);
        lock_guard g(m);
        {
            docids.resize(index.doc_size());
            iota(docids.begin(), docids.end(), 0);
            counts.assign(index.doc_size(), 0);
            vector<pair<size_t, size_t>> res;

            for (string_view word : words) {
                for (const auto& [docid, cnt] : index.Lookup(word, res)) {
                    counts[docid] += cnt;
                }
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
            if (counts[docid] == 0) {
                break;
            }
            search_results_output << " {"
                << "docid: " << docid << ", "
                << "hitcount: " << counts[docid] << '}';
        }
        search_results_output << endl;
    }
}

SearchServer::SearchServer(istream& document_input)
{
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input)
{
    futures.push_back(
        async(UpdateDocumentBaseInner, ref(document_input), ref(index), ref(m))
    );
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output)
{
    futures.push_back(
        async(AddQueriesStreamInner, ref(query_input), ref(search_results_output), ref(index), ref(m))
    );
}

