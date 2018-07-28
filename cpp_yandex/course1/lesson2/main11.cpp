#include <iostream>
#include <string>
#include <map>

using namespace std;

map<char, int>
BuildCharCounters(string s)
{
	map<char, int> charCounter;
	for (auto ch : s)
		charCounter[ch]++;
	return charCounter;
}

int
main()
{
	int n;
	cin >> n;
	for (int i = 0; i < n; i++) {
		string w1, w2;
		cin >> w1 >> w2;
		map<char, int> charCounter1 = BuildCharCounters(w1);
		map<char, int> charCounter2 = BuildCharCounters(w2);
		if (charCounter1 == charCounter2)
			cout << "YES" << endl;
		else
			cout << "NO" << endl;
	}

	return 0;
}
