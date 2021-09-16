#include <iostream>
#include <vector>

using namespace std;

vector<int>
Reversed(const vector<int>& v)
{
	vector<int> res;
	for (long long i = v.size() - 1; i >= 0; i--)
		res.push_back(v[i]);
	return res;
}

int main()
{
	vector<int> numbers = {1, 5, 3, 4, 2};
	vector<int> res = Reversed(numbers);

	for (auto n : res)
		cout << n << " ";

	return 0;
}
