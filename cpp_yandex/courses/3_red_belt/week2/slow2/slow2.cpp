#include <iomanip>
#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

class ReadingManager {
public:
  ReadingManager()
      : user_page_counts_(MAX_USER_COUNT_ + 1, 0)
      , pages(MAX_PAGE_COUNT_ + 1, 0)
      , user_count_(0)
  {
  }

  void Read(int user_id, int page_count) {
      int old_page_count = user_page_counts_[user_id];
      if (old_page_count == 0) {
          ++user_count_;
      }

      user_page_counts_[user_id] = page_count;
      if (old_page_count > 0) {
          --pages[old_page_count];
      }
      ++pages[page_count];
  }

  double Cheer(int user_id) const {
      int page_count = user_page_counts_[user_id];
      if (page_count == 0) {
          return 0;
      }
      if (user_count_ == 1) {
          return 1;
      }

      int page_sum = 0;
      for (int i = 0; i < page_count; ++i) {
          page_sum += pages[i];
      }

      return page_sum * 1.0 / (user_count_ - 1);
  }

private:
  static const int MAX_USER_COUNT_ = 100'000;
  static const int MAX_PAGE_COUNT_ = 1'000;

  vector<int> user_page_counts_;
  vector<int> pages;
  int user_count_;
};


int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ReadingManager manager;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;
    int user_id;
    cin >> user_id;

    if (query_type == "READ") {
      int page_count;
      cin >> page_count;
      manager.Read(user_id, page_count);
    } else if (query_type == "CHEER") {
      cout << setprecision(6) << manager.Cheer(user_id) << "\n";
    }
  }

  return 0;
}