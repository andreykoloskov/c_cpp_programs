#include "test_runner.h"

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

using namespace std;

struct Record {
  string id;
  string title;
  string user;
  int timestamp;
  int karma;
};

// Реализуйте этот класс
class Database {
public:
  bool Put(const Record& record) {
    auto [it, result] = records_.insert({ record.id, {record, {}, {}, {}} });
    if (!result) {
      return false;
    }

    it->second.timestamp_it = records_by_timestamp_.insert({ record.timestamp, &it->second.record });
    it->second.karma_it = records_by_karma_.insert({ record.karma, &it->second.record });
    it->second.user_it = records_by_user_.insert({ record.user, &it->second.record });

    return true;
  }

  const Record* GetById(const string& id) const {
    auto it = records_.find(id);
    if (it == records_.end()) {
      return nullptr;
    }
    return &it->second.record;
  }

  bool Erase(const string& id) {
    auto it = records_.find(id);
    if (it == records_.end()) {
      return false;
    }

    records_by_timestamp_.erase(it->second.timestamp_it);
    records_by_karma_.erase(it->second.karma_it);
    records_by_user_.erase(it->second.user_it);
    records_.erase(it);
    return true;
  }

  template <typename Container, typename Key, typename Callback>
  bool KeyRangIteration(Container& container, Key& key, Callback callback) const {
      auto [it1, it2] = container.equal_range(key);
      for (auto it = it1; it != it2; ++it) {
          if (!callback(*it->second)) {
            return false;
          }
      }
      return true;
  }
  
  template <typename Container, typename Callback>
  void KeyRangIterations(int low, int high, Container& container, Callback callback) const {
    for (int i = low; i <= high; ++i) {
      if (!KeyRangIteration(container, i, callback)) {
        return;
      }
    }
  }
  
  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    KeyRangIterations(low, high, records_by_timestamp_, callback);
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    KeyRangIterations(low, high, records_by_karma_, callback);
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    KeyRangIteration(records_by_user_, user, callback);
  }

private:
  template<typename T>
  using Data_map = multimap<T, const Record*>;

  struct Data {
    Record record;
    Data_map<int>::iterator timestamp_it;
    Data_map<int>::iterator karma_it;
    Data_map<string>::iterator user_it;
  };

  unordered_map<string, Data> records_;
  Data_map<int> records_by_timestamp_;
  Data_map<int> records_by_karma_;
  Data_map<string> records_by_user_;
};

void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({"id1", "Hello there", "master", 1536107260, good_karma});
  db.Put({"id2", "O>>-<", "general2", 1536107260, bad_karma});

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({"id1", "Don't sell", "master", 1536107260, 1000});
  db.Put({"id2", "Rethink life", "master", 1536107260, 2000});

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({"id", "Have a hand", "not-master", 1536107260, 10});
  db.Erase("id");
  db.Put({"id", final_body, "not-master", 1536107260, -10});

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}