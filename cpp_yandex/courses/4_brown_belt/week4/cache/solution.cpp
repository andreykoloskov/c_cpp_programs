#include "Common.h"
#include <unordered_map>
#include <utility>
#include <algorithm>
#include <mutex>

using namespace std;

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) : books_unpacker_(books_unpacker), settings_(settings) {
  }

  BookPtr GetBook(const string& book_name) override {
    lock_guard g(m);
    auto it = books_.find(book_name);
    if (it != books_.end()) {
      it->second.second = ++max_value;
      return it->second.first;
    }

    BookPtr bookPtr = books_unpacker_->UnpackBook(book_name);
    auto sz = bookPtr->GetContent().size();
    if (sz <= settings_.max_memory) {
      while (sz > settings_.max_memory - memory_) {
        auto it2 = min_element(books_.begin(), books_.end(), [](const auto& a, const auto& b) {
          return a.second.second < b.second.second;
        });
        memory_ -= it2->second.first->GetContent().size();
        books_.erase(it2);
      }

      memory_ += sz;
      books_[book_name] = make_pair(bookPtr, ++max_value);
    }

    return bookPtr;
  }

private:
  shared_ptr<IBooksUnpacker> books_unpacker_;
  const Settings settings_;
  unordered_map<string, pair<BookPtr, int>> books_;
  int max_value = 0;
  size_t memory_ = 0;
  mutable mutex m;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return make_unique<LruCache>(books_unpacker, settings);
}