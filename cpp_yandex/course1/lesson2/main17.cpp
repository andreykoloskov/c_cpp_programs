#include <iostream>
#include <set>
#include <map>
#include <string>

using namespace std;

int
main()
{
	int q;
	cin >> q;
	map<string, set<string>> dictionary;

	for (int i = 0; i < q; i++) {
		string op;
		cin >> op;

		if (op == "ADD") {
			string word1, word2;
			cin >> word1 >> word2;
			dictionary[word1].insert(word2);
			dictionary[word2].insert(word1);
		} else if (op == "COUNT") {
			string word;
			cin >> word;
			if (dictionary.count(word) == 0)
				cout << 0 << endl;
			else
				cout << dictionary[word].size() << endl;
		} else if (op == "CHECK") {
			string word1, word2;
			cin >> word1 >> word2;
			if ((dictionary.count(word1) == 1
					&& dictionary[word1].count(word2) == 1)
					|| (dictionary.count(word2) == 1
					&& dictionary[word2].count(word1) == 1))
				cout << "YES" << endl;
			else
				cout << "NO" << endl;
		}
	}

	return 0;
}
