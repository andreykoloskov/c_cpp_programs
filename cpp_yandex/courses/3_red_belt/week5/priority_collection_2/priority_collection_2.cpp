#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <list>
#include <map>

using namespace std;

template <typename T>
class PriorityCollection {
public:
    /* тип, используемый для идентификаторов */
    using PriorityPair = pair<int, int>;
    using Id = typename list<pair<PriorityPair,T>>::iterator;

    // Добавить объект с нулевым приоритетом
    // с помощью перемещения и вернуть его идентификатор
    Id Add(T object)
    {
        PriorityPair pp = make_pair(0, max_number_);
        ++max_number_;
        Id id = elements_.insert(elements_.end(), make_pair(pp, move(object)));
        priorities_[pp] = id;
        return id;
    }

    // Добавить все элементы диапазона [range_begin, range_end)
    // с помощью перемещения, записав выданные им идентификаторы
    // в диапазон [ids_begin, ...)
    template <typename ObjInputIt, typename IdOutputIt>
    void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin)
    {
        auto it_res = ids_begin;
        for (auto it = range_begin; it != range_end; ++it) {
            Id id = Add(move(*it));
            *it_res = id;
            ++it_res;
        }
    }

    // Определить, принадлежит ли идентификатор какому-либо
    // хранящемуся в контейнере объекту
    bool IsValid(Id id) const
    {
        return priorities_.find(id->first) != priorities_.end();
    }

    // Получить объект по идентификатору
    const T& Get(Id id) const
    {
        return id->second;
    }

    // Увеличить приоритет объекта на 1
    void Promote(Id id)
    {
        PriorityPair pp = id->first;
        PriorityPair pp_new = make_pair(pp.first + 1, pp.second);
        priorities_.erase(pp);
        id->first = pp_new;
        priorities_[pp_new] = id;
    }

    // Получить объект с максимальным приоритетом и его приоритет
    pair<const T&, int> GetMax() const
    {
        auto it = priorities_.rbegin();
        return { it->second->second, it->first.first };
    }

    // Аналогично GetMax, но удаляет элемент из контейнера
    pair<T, int> PopMax()
    {
        auto it = priorities_.rbegin();
        T elem = move(it->second->second);
        int priority = it->first.first;
        elements_.erase(it->second);
        priorities_.erase(it->first);
        return { move(elem), priority };
    }

private:
    // Приватные поля и методы
    list<pair<PriorityPair, T>> elements_;
    map<PriorityPair, Id> priorities_;
    int max_number_ = 0;
};


class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}