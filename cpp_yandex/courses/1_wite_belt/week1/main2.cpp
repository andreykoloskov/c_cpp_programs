/*
В стандартном потоке даны три строки, разделённые пробелом. Каждая строка состоит из строчных латинских букв и имеет длину не более 30 символов. Выведите в стандартный вывод лексикографически минимальную из них.
*/

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	vector<string> v(3);
	cin >> v[0] >> v[1] >> v[2];
	sort(begin(v), end(v));
	cout << v[0];
	return 0;
}
