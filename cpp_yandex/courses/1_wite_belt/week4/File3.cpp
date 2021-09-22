/*
В файле input.txt записаны вещественные числа, по одному на строчку. На стандартный вывод напечатайте эти числа в том же порядке, по одному на строке, но сделайте так, чтобы у каждого из них было ровно три знака после десятичной точки.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

int main() {
    ifstream input("input.txt");
    double d;
    cout << fixed << setprecision(3);
    while (input >> d) {
        cout << d << endl;
    }
}
