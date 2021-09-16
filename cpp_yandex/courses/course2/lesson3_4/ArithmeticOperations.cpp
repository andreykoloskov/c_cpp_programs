#include <iostream>
#include <string>
#include <deque>

using namespace std;

int main() {
    deque<string> res;
    int number;
    cin >> number;
    res.push_back(to_string(number));
    int operations_num;
    cin >> operations_num;

    for (int i = 0; i < operations_num; i++) {
        char op;
        int num;
        cin >> op >> num;
        res.push_front("(");
        res.push_back(")");
        res.push_back(" " + string(1, op) + " " + to_string(num));
    }

    for (auto& r : res)
        cout << r;
    cout << endl;
}