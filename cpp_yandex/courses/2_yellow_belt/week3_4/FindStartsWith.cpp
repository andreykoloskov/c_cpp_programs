/*
�������� ������� FindStartsWith:

����������� ��������������� ����� ����� � ���� ���������� range_begin, range_end � ���� ������ prefix;

������������ �������� �����, ������������ � ������� prefix, � ���� ���� ����������.

���� �������� �������� ����, ��� ������� ������ ��������� �� �� ����� � ����������, ���� ����� ��� ��������� ������� ���������� �������� ����� ������, ������������ � ������� prefix (������� ��������� equal_range). �������������, ��� ������ ������� ���� �� �������� ��������� ���� � ������ prefix ����� �������� �������� ��������� ������.

����� ������ �������������� �� ��������������� ��������� � ��������, � ������� ��������� ������.

���������
� �������� (char) ����� ���������� ��� �������� �����, ������� ����� ������� ��������� ��� ���������� ����� � ���������� �������. ��������, ��� ������ s ��������� --s[0] ������� � ������ ����� �� ����������.

�������� ��������, ��� ��������� 'a' + 1 ����� ��� int � ������� ����� ������������ �������� ��� � ���� char � ������� static_cast.
*/

#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

template <typename RandomIt>
pair<RandomIt, RandomIt> FindStartsWith(
        RandomIt range_begin, RandomIt range_end,
        char prefix) {
    auto first = lower_bound(range_begin, range_end, string(1, prefix));
    auto second = lower_bound(range_begin, range_end,
            string(1, static_cast<char>(++prefix)));
    return make_pair(first, second);
};

int main() {
    const vector<string> sorted_strings = {"moscow", "murmansk", "vologda"};

    const auto m_result =
            FindStartsWith(begin(sorted_strings), end(sorted_strings), 'm');
    for (auto it = m_result.first; it != m_result.second; ++it) {
        cout << *it << " ";
    }
    cout << endl;

    const auto p_result =
            FindStartsWith(begin(sorted_strings), end(sorted_strings), 'p');
    cout << (p_result.first - begin(sorted_strings)) << " " <<
         (p_result.second - begin(sorted_strings)) << endl;

    const auto z_result =
            FindStartsWith(begin(sorted_strings), end(sorted_strings), 'z');
    cout << (z_result.first - begin(sorted_strings)) << " " <<
         (z_result.second - begin(sorted_strings)) << endl;

    return 0;
}