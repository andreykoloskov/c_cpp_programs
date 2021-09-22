/*
Условие
В стандартном потоке дана одна строка, состоящая из числа N и следующих за ним N строк S. Между собой число и строки разделены пробелом.

Отсортируйте строки S в лексикографическом порядке по возрастанию, игнорируя регистр букв, и выведите их в стандартный поток вывода.

Ограничения
0 <= N <= 1000

1 <= |S| <= 15

Каждая строка S[i] может состоять из следующих символов: [0-9,a-z,A-Z]

Подсказка
Обратите внимание на функцию tolower.
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
