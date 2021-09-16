#include <iostream>
#include <map>
#include <stdexcept>

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
