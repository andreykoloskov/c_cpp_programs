/*
Часть 2. Без лишних скобок
Модифицируйте решение предыдущей части так, чтобы предыдущее выражение обрамлялось скобками лишь при необходимости, то есть только в том случае, когда очередная операция имеет бо́льший приоритет, чем предыдущая.
*/

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

    char op1 = '&', op2;

    for (int i = 0; i < operations_num; i++) {
        int num;
        cin >> op2 >> num;

        if ((op1 == '+' || op1 == '-') && (op2 == '*' || op2 == '/')) {
            res.push_front("(");
            res.push_back(")");
        }

        res.push_back(" " + string(1, op2) + " " + to_string(num));
        op1 = op2;
    }

    for (auto& r : res)
        cout << r;
    cout << endl;
}
