#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    // Напишите реализацию функции,
    // не копируя сортируемые элементы

    if (range_end - range_begin < 2)
        return;

    vector<typename RandomIt::value_type> elements(make_move_iterator(range_begin), make_move_iterator(range_end));
    long long middle3 = (elements.end() - elements.begin()) / 3;
    MergeSort(elements.begin(), elements.begin() + middle3);
    MergeSort(elements.begin() + middle3, elements.begin() + 2 * middle3);
    MergeSort(elements.begin() + 2 * middle3, elements.end());

    vector<typename  RandomIt::value_type> elements2;
    merge(make_move_iterator(elements.begin()), make_move_iterator(elements.begin() + middle3),
            make_move_iterator(elements.begin() + middle3), make_move_iterator(elements.begin() + 2 * middle3),
            back_inserter(elements2));
    merge(make_move_iterator(elements2.begin()), make_move_iterator(elements2.end()),
            make_move_iterator(elements.begin() + 2 * middle3), make_move_iterator(elements.end()), range_begin);
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}