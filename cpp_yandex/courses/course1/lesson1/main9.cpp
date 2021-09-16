#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	int n;
	cin >> n;
	vector<int> v;

	while (n > 0) {
		v.push_back(n % 2);
		n = n / 2;
	}

	for (long long i = v.size() - 1; i >= 0; i--)
		cout << v[i];
	cout << endl;


	return 0;
}
