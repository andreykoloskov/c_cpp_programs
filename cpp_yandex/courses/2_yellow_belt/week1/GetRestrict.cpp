/*
Реализуйте шаблонную функцию GetRefStrict, которая на вход принимает: map и значение ключа k. Если элемент по ключу k в коллекции отсутствует, то функция должна бросить исключение runtime_error, иначе вернуть ссылку на элемент в коллекции.
*/

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

using namespace std;


template <typename K, typename V>
V& GetRefStrict(map<K, V>& m, K k) {
    if (m.find(k) == m.end()) {
        throw runtime_error("");
    }
    return m[k];
};

int main() {
    map<int, string> m = {{0, "value"}};
    string& item = GetRefStrict(m, 0);
    item = "newvalue";
    cout << m[0] << endl; // выведет newvalue

    return 0;
}
