/*
ƒано целое положительное число N, не превышающее 9. ¬ыведите все перестановки чисел от 1 до N в обратном лексикографическом пор€дке (см. пример).
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintVector(vector<int>& v) {
    for (int it : v)
        cout << it << " ";
    cout << endl;
}

int main() {
    int n;
    cin >> n;
    vector<int> v(n);
    for (int i = 0; i < n; i++)
        v[i] = n - i;
    PrintVector(v);

    while (prev_permutation(begin(v), end(v)))
        PrintVector(v);
}