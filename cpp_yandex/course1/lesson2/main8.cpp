/*
#include <iostream>
#include <vector>

using namespace std;

int
main()
{
	int n;
	cin >> n;
	vector<int> t(n);
	int avg = 0;
	for (int i = 0; i < n; i++) {
		cin >> t[i];
		avg += t[i];
	}
	avg /= n;

	vector<int> big_t;
	int k = 0;
	for (int i = 0; i < n; i++) {
		if (t[i] > avg) {
			big_t.push_back(i);
			k++;
		}
	}

	cout << k << endl;
	for (auto b : big_t)
		cout << b << " ";
	cout << endl;

	return 0;
}
*/
