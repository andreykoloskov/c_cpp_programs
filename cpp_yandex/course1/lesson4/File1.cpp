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
