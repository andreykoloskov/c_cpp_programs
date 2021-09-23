/*
Часть 2. Реализация с разбиением на 3 части
Реализуйте сортировку слиянием, разбивая диапазон на 3 равные части, а не на 2. Гарантируется, что длина исходного диапазона является степенью 3.

Соответственно, пункты 3–5 алгоритма нужно заменить следующими:

Разбить вектор на 3 равные части.

Вызвать функцию MergeSort от каждой части вектора.

Слить первые две трети вектора с помощью алгоритма merge, сохранив результат во временный вектор с помощью back_inserter.

Слить временный вектор из предыдущего пункта с последней третью вектора из п. 2, записав полученный отсортированный диапазон вместо исходного.
*/

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
