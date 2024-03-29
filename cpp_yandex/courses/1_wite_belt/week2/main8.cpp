﻿/*
Даны значения температуры, наблюдавшиеся в течение N подряд идущих дней. Найдите номера дней (в нумерации с нуля) со значением температуры выше среднего арифметического за все N дней.

Гарантируется, что среднее арифметическое значений температуры является целым числом.

Формат ввода
Вводится число N, затем N неотрицательных целых чисел — значения температуры в 0-й, 1-й, ... (N−1)-й день.

Формат вывода
Первое число K — количество дней, значение температуры в которых выше среднего арифметического. Затем K целых чисел — номера этих дней.

Пример

Ввод
5
7 6 3 0 9

Вывод
3
0 1 4
*/

#include <iostream>
#include <vector>

using namespace std;

int
main()
{
	int n;
	cin >> n;
	vector<int> t(n);
	int avg = 0;
	for (int i = 0; i < n; i++) {
		cin >> t[i];
		avg += t[i];
	}
	avg /= n;

	vector<int> big_t;
	int k = 0;
	for (int i = 0; i < n; i++) {
		if (t[i] > avg) {
			big_t.push_back(i);
			k++;
		}
	}

	cout << k << endl;
	for (auto b : big_t)
		cout << b << " ";
	cout << endl;

	return 0;
}
