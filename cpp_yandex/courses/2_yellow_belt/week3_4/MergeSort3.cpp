#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
    if (range_end - range_begin < 2)
        return;

    vector<typename RandomIt::value_type> elements(range_begin, range_end);
    long long middle3 = (elements.end() - elements.begin()) / 3;
    MergeSort(elements.begin(), elements.begin() + middle3);
    MergeSort(elements.begin() + middle3, elements.begin() + 2 * middle3);
    MergeSort(elements.begin() + 2 * middle3, elements.end());

    vector<typename  RandomIt::value_type> elements2;
    merge(elements.begin(), elements.begin() + middle3,
            elements.begin() + middle3, elements.begin() + 2 * middle3,
            back_inserter(elements2));
    merge(elements2.begin(), elements2.end(),
            elements.begin() + 2 * middle3, elements.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1, 5};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}
