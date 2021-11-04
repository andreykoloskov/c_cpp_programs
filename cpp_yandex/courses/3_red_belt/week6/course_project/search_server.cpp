#include <algorithm>
#include <iterator>
#include <sstream>
#include "search_server.h"
#include "iterator_range.h"
#include "utilities.h"


SearchServer::SearchServer(istream& document_input)
{
    UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input)
{
    future<InvertedIndex> asynch_index = async([&document_input] {
        InvertedIndex result;
        for (string current_document; getline(document_input, current_document); ) {
            result.Add(current_document);
        }
        return move(result);
    });

    InvertedIndex new_index = move(asynch_index.get());
    {
        lock_guard g(m);
        index = move(new_index);
    }
}

void SearchServer::AddQueriesStream(istream& query_input, ostream& search_results_output)
{
    vector<Element> result;
    result.reserve(query_max_size);
    vector<future<void>> futures;

    for (string current_query; getline(query_input, current_query); ) {
        result.push_back({ move(current_query), 0, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0} });
    }

    size_t current_size = result.size();
    size_t page_cnt = result.size() / 8 + ((result.size() % 8) ? 1 : 0);
    auto it1 = result.begin();
    auto it2 = result.begin() + page_cnt;

    while (it1 != result.end()) {
        futures.push_back(async([this, it1, it2] {
            vector<int> docids(max_size);
            vector<size_t> counts(max_size);

            for (auto it = it1; it != it2; ++it) {
                iota(docids.begin(), docids.end(), 0);
                counts.assign(counts.size(), 0);
                vector<pair<size_t, size_t>> res;

                {
                    for (string_view word : SplitIntoWords(it->current_query)) {
                        lock_guard g(m);
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

                for (auto docid : Head(docids, dist)) {
                    if (counts[docid] > 0) {
                        it->docids[it->quantity] = docid;
                        it->hitcounts[it->quantity] = counts[docid];
                        ++it->quantity;
                    }
                }
            }
        }));

        it1 = it2;
        it2 = (result.end() - it2 >= page_cnt) ? it1 + page_cnt : result.end();
    }

    for (auto& f : futures) {
        f.get();
    }

    for (const auto& element : result) {
        search_results_output << element.current_query << ':';
        for (size_t i = 0; i < element.quantity; ++i) {
            search_results_output << " {"
                << "docid: " << element.docids[i] << ", "
                << "hitcount: " << element.hitcounts[i] << '}';
        }
        search_results_output << endl;
    }
}

