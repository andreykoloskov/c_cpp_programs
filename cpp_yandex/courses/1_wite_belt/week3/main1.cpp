/*
� ����������� ������ ���� ���� ������, ��������� �� N + 1 ����� �����. ������ ������ ��� ���� ����� N. ����� ������� ��� N �����, ��������� �� �� ������ A. ����� ����� ����� ��������� ��������.

������������ ������ � �� ������ � �������� ��� � ����������� �����.

�����������
0 <= N <= 1000

-1000000 <= A[i] <= 1000000

���������
������� sort ������������� ��������� ������ ��������. ������� ��� ��������� �� ������.
*/

#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

int
main()
{
	int n;
	cin >> n;
	vector<int> numbers(n);
	for (auto& i : numbers)
		cin >> i;

	sort(begin(numbers), end(numbers),
			[](int a, int b) { return abs(a) < abs(b); });

	for (const auto& i : numbers)
		cout << i << " ";
	cout << endl;

	return 0;
}
