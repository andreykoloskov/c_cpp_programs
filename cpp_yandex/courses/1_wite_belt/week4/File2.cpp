/*
Снова считайте все содержимое файла input.txt, но на этот раз выведите его в файл output.txt . Точно так же гарантируется, что содержимое файла input.txt заканчивается переводом строки.
*/

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string path = "input.txt";
    string path2 = "output.txt";
    ifstream input(path);
    ofstream output(path2);
    if (input) {
        string str;
        while (getline(input, str)) {
            output << str << endl;
        }
    }
}
