#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

vector<string> SplitIntoWords(const string& s) {
    vector<string> res;
    for (auto it1 = begin(s), it2 = find(it1, end(s), ' '); it1 != it2;
            it1 = ++it2, it2 = find(it1, end(s), ' ')) {
        res.push_back(string(it1, it2));
        if (it2 == end(s))
            break;
    }

    return res;
}

int main() {
    string s = "C Cpp Java Python";

    vector<string> words = SplitIntoWords(s);
    cout << words.size() << " ";
    for (auto it = begin(words); it != end(words); ++it) {
        if (it != begin(words)) {
            cout << "/";
        }
        cout << *it;
    }
    cout << endl;

    return 0;
}
