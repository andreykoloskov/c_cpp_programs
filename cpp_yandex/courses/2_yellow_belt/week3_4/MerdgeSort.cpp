/*
�������� ��������� ������� MergeSort, ����������� ��� ��������� ���������� ���� RandomIt � ����������� �������� ��� �������� � ������� ���������� ��������. �������������, ���:

��������� ���� RandomIt ���������� �� ���������������� ���������� ������� � ������, �� ���� �� ����� ���������� � ������� ���������� <, <=, > � >=, � ����� �������� � ���������� � �������;

����������� ������� ����� ���������� � ������� ��������� <.

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end);

����� 1. ���������� � ���������� �� 2 �����
��������
������������ �������� ���������� �������� �������� ��������� �������:

���� �������� �������� ������ 2 ���������, ����� �� �������.

������� ������, ���������� ��� �������� �������� ���������.

������� ������ �� ��� ������ �����. (� ���� ������ �������������, ��� ����� ������������� ��������� �������� �������� ������, ��� ��� ������ ������ ����� ������� �� ��� ������ �����.)

������� ������� MergeSort �� ������ �������� �������.

� ������� ��������� std::merge ����� ��������������� ��������, ������� ���������� ��������������� �������� ������ ���������.

�� ������ ����������� ������ ���� �������� � ������� ������: ����������� ������� ����� ���������, ��� �� ���������� � ���������� ������ ��� ��������.

���������
����� ������� ������, ���������� ��� �������� �������� ��������� (�. 2 ���������), ���������� ����� �� ���� ��������� �������� ��� ���������, �� ������� �� ���������. ���� �������� RandomIt ����������� ������������ ���������� (�������, ������, ���������, �������...), ����������� ��� ����� �������� � ������� ��������� typename RandomIt::value_type. ����� �������, �������������, ��� ������� ������ � �. 2 ����� ��������� �������:

vector<typename RandomIt::value_type> elements(range_begin, range_end);
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
    long long middle = (elements.end() - elements.begin()) / 2;
    MergeSort(elements.begin(), elements.begin() + middle);
    MergeSort(elements.begin() + middle, elements.end());

    merge(elements.begin(), elements.begin() + middle,
            elements.begin() + middle, elements.end(), range_begin);
}

int main() {
    vector<int> v = {6, 4, 7, 6, 4, 4, 0, 1};
    MergeSort(begin(v), end(v));
    for (int x : v) {
        cout << x << " ";
    }
    cout << endl;
    return 0;
}