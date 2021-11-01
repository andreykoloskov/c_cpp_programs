#include "test_runner.h"

#include <algorithm>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>

using namespace std;

// ��������� Group<String> ��� ������������� ���� String
// ��������� vector<String>.
// ��������� ����� � �������� ������������� ��������
// ������� ����� ������� �� ������������ ������ ��������,
// � ������ ����� � vector<Group<String>>.
template <typename String>
using Group = vector<String>;

// ��� ���� ��������� ������� ����
// ��������� ������ ����������� typename String::value_type
// ������������ Char<String>
template <typename String>
using Char = typename String::value_type;

template <typename IteratorT>
class Comparator
{
public:
    bool operator()(const IteratorT& lhs, const IteratorT& rhs) const { return *lhs < *rhs; }
};

template <typename T>
class Comparator2
{
public:
    bool operator()(const T& lhs, const T& rhs) const
    {
        size_t min_size = lhs.size() < rhs.size() ? lhs.size() : rhs.size();
        for (size_t i = 0; i < min_size; ++i) {
            if (*(lhs[i]) < *(rhs[i])) {
                return true;
            }
            else if (*(lhs[i]) > *(rhs[i])) {
                return false;
            }
        }
        return lhs.size() < rhs.size();
    }
};

template <typename String>
vector<Group<String>> GroupHeavyStrings(vector<String> strings) {
  // �������� ���������� �������,
  // ����������� �� ����� 1 ����������� ������� �������

    using CharIterator = typename String::iterator;
    using VectorIterator = typename vector<String>::iterator;

    vector<Group<String>> result;
    map<vector<CharIterator>, vector<VectorIterator>, Comparator2<vector<CharIterator>>> data;

    for (auto it = strings.begin(); it != strings.end(); ++it) {
        set<CharIterator, Comparator<CharIterator>> s;
        for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
            s.insert(it2);
        }
        vector<CharIterator> s_res;
        move(s.begin(), s.end(), back_inserter(s_res));
        data[s_res].push_back(it);
    }

    for (auto& d : data) {
        Group<String> r;
        for (auto& d0 : d.second) {
            r.push_back(move(*d0));
        }
        result.push_back(move(r));
    }
    return result;
}


void TestGroupingABC() {
  vector<string> strings = {"caab", "abc", "cccc", "bacc", "c"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 2);
  sort(begin(groups), end(groups));  // ������� ����� �� ����� ��������
  ASSERT_EQUAL(groups[0], vector<string>({"caab", "abc", "bacc"}));
  ASSERT_EQUAL(groups[1], vector<string>({"cccc", "c"}));
}

void TestGroupingReal() {
  vector<string> strings = {"law", "port", "top", "laptop", "pot", "paloalto", "wall", "awl"};
  auto groups = GroupHeavyStrings(strings);
  ASSERT_EQUAL(groups.size(), 4);
  sort(begin(groups), end(groups));  // ������� ����� �� ����� ��������
  ASSERT_EQUAL(groups[0], vector<string>({"laptop", "paloalto"}));
  ASSERT_EQUAL(groups[1], vector<string>({"law", "wall", "awl"}));
  ASSERT_EQUAL(groups[2], vector<string>({"port"}));
  ASSERT_EQUAL(groups[3], vector<string>({"top", "pot"}));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestGroupingABC);
  RUN_TEST(tr, TestGroupingReal);
  return 0;
}