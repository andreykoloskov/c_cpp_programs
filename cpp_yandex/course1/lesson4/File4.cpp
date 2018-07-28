#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

int main() {
    ifstream input("input.txt");
    int n, m, k;
    input >> n >> m;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            input >> k;
            input.ignore(1);
            cout << setw(10) << k;
            if (j < m - 1) {
                cout << ' ';
            }

        }
        cout << endl;
    }
}
