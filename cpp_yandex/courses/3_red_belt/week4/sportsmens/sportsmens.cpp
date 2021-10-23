#include <iostream>
#include <algorithm>
#include <list>
#include <map>

using namespace std;

int main()
{
    list<int> lst;
    map<int, list<int>::iterator> lst_searcher;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int first, second;
        cin >> first >> second;
        auto searcher = lst_searcher.find(second);
        auto it = lst.insert(searcher != lst_searcher.end() ? searcher->second : lst.end(), first);
        lst_searcher[first] = it;
    }

    for (auto l : lst) {
        cout << l << '\n';
    }

    return 0;
}