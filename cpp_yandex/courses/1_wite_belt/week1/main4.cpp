/*
���� ��� ����� ����� A � B � ��������� �� 0 �� 1 000 000 ������������. �������� ���������, ������� ��������� ����� ����� �������� �� ������� A �� B.

���� B = 0, �������� "Impossible".
*/

#include <iostream>

using namespace std;

int main()
{
	int a, b;
	cin >> a >> b;

	if (b == 0)
		cout << "Impossible" << endl;
	else
		cout << a / b << endl;

	return 0;
}
