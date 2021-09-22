/*
Напишите функцию MoveStrings, которая принимает два вектора строк, source и destination, и дописывает все строки из первого вектора в конец второго. После выполнения функции вектор source должен оказаться пустым.

Чтобы очистить содержимое вектора, надо вызвать у него метод clear:

vector<string> words = {"hey", "hi"};
words.clear();
// Теперь вектор words пуст

Пример
vector<string> source = {"a", "b", "c"};
vector<string> destination = {"z"};
MoveStrings(source, destination);
// source должен оказаться пустым
// destination должен содержать строки "z", "a", "b", "c" именно в таком порядке
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

void
MoveStrings(vector<string>& src, vector<string>& dst)
{
	for (auto w : src)
		dst.push_back(w);
	src.clear();
}

int main()
{
	vector<string> source = {"a", "b", "c"};
	vector<string> destination = {"z"};
	MoveStrings(source, destination);
	cout << "source" << endl;
	for (auto w : source)
		cout << w << " ";
	cout << endl << "destination" << endl;
	for (auto w : destination)
		cout << w << " ";
	cout << endl;
	return 0;
}
