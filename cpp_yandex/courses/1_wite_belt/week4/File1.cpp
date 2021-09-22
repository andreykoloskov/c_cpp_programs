/*
В этом задании вам предстоит написать две небольшие программы. Каждый пункт - отдельная задача, решение отправляйте в поле с соответствующим номером.

Часть 1
Ваша программа должна считать содержимое файла input.txt и напечатать его на экран без изменений. Гарантируется, что содержимое файла input.txt заканчивается переводом строки.
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string path = "input.txt";
    ifstream input(path);
    if (input) {
        string str;
        while (getline(input, str)) {
            cout << str << endl;
        }
    }
}
