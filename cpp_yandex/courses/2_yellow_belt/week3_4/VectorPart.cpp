/*
Напишите функцию PrintVectorPart, принимающую вектор целых чисел numbers, выполняющую поиск первого отрицательного числа в нём и выводящую в стандартный вывод все числа, расположенные левее найденного, в обратном порядке. Если вектор не содержит отрицательных чисел, выведите все числа в обратном порядке.
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void PrintVectorPart(const vector<int>& numbers) {
    auto it = find_if(begin(numbers), end(numbers),
            [](int a) {
                return a < 0;
            });

    for (auto itt = it; itt != begin(numbers);)
        cout << *(--itt) << " ";
    cout << endl;
}

int main() {
    PrintVectorPart({6, 1, 8, -5, 4});
    PrintVectorPart({-6, 1, 8, -5, 4});  // ничего не выведется
    PrintVectorPart({6, 1, 8, 5, 4});
    PrintVectorPart({});
    return 0;
}
