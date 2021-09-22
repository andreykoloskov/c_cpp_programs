/*
��� ����� �����. ������� ���������� ���������� ����� � ���� ������.

������ �����
������� �������� ���������� ����� N, ����� � ���� N �����, ���������� �������� �������� ������. ��� ������ ������� ���� �� ��������� ����, ���� � �������� �������������.

������ ������
�������� ������������ ����� ����� � ���������� ���������� ����� � ������ ������.

������
����
6
first
second
first
third
second
second

�����
3
*/

#include <iostream>
#include <set>
#include <string>

using namespace std;

int
main()
{
	int n;
	cin >> n;
	set<string> strings;
	for (int i = 0; i < n; i++) {
		string str;
		cin >> str;
		strings.insert(str);
	}

	cout << strings.size() << endl;

	return 0;
}
