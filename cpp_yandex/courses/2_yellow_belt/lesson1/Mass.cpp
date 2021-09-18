#include <iostream>

using namespace std;

int main() {
    uint64_t n, r;
    uint64_t sum = 0;

    cin >> n >> r;
    for (int i = 0; i < n; i++) {
        uint64_t w, h, d;
        cin >> w >> h >> d;
        sum += w * h * d;
    }

    sum *= r;
    cout << sum << endl;

    return 0;
}