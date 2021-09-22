/*
В стандартном потоке дана одна строка, состоящая из N + 1 целых чисел. Первым числом идёт само число N. Далее следуют ещё N чисел, обозначим их за массив A. Между собой числа разделены пробелом.

Отсортируйте массив А по модулю и выведите его в стандартный поток.

Ограничения
0 <= N <= 1000

-1000000 <= A[i] <= 1000000

Подсказка
функция sort дополнительно принимает третий аргумент. Изучите его подробнее по ссылке.
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
