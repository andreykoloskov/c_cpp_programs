/*
�������
� ����������� ������ ���� ���� ������, ��������� �� ����� N � ��������� �� ��� N ����� S. ����� ����� ����� � ������ ��������� ��������.

������������ ������ S � ������������������ ������� �� �����������, ��������� ������� ����, � �������� �� � ����������� ����� ������.

�����������
0 <= N <= 1000

1 <= |S| <= 15

������ ������ S[i] ����� �������� �� ��������� ��������: [0-9,a-z,A-Z]

���������
�������� �������� �� ������� tolower.
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

using namespace std;

int
main()
{
	int n;
	cin >> n;
	vector<string> strings(n);
	for (auto& str : strings)
		cin >> str;

	sort(begin(strings), end(strings),
			[](string st1, string st2) {
		string st10, st20;
		for (auto s : st1)
			st10 += tolower(s);
		for (auto s : st2)
			st20 += tolower(s);
		return st10 < st20;
	});

	for (const auto& st : strings)
		cout << st << " ";
	cout << endl;

	return 0;
}
