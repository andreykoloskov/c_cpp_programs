#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> tmp(n);
    int64_t sum = 0;
    for (int& t : tmp) {
        cin >> t;
        sum += t;
    }
    double avg = sum / n;

    vector<int> res;
    int cnt = 0;
    for (int i = 0; i < n; i++) {
        if (tmp[i] > avg) {
            res.push_back(i);
            cnt++;
        }
    }

    cout << cnt << endl;
    for (int t : res)
        cout << t << " ";
    cout << endl;

    return 0;
}