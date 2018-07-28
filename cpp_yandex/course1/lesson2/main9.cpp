#include <iostream>
#include <vector>
#include <string>

using namespace std;

void
come(vector<int>& que, int k)
{
	if (k > 0) {
		for (int i = 0; i < k; i++)
			que.push_back(0);
	} else if (k < 0) {
		for (int i = k; i < 0; i++)
			que.pop_back();
	}
}

void
worry(vector<int>& que, int k)
{
	que[k] = 1;
}

void
quiet(vector<int>& que, int k)
{
	que[k] = 0;
}

void worry_count(vector<int>& que)
{
	int cnt = 0;
	for (auto w : que)
		if (w == 1)
			cnt++;
	cout << cnt << endl;
}

int
main()
{
	int q;
	cin >> q;
	vector<int> que;
	for (int i = 0; i < q; i++) {
		string op;
		int k;
		cin >> op;
		if (op == "COME") {
			cin >> k;
			come(que, k);
		} else if (op == "WORRY") {
			cin >> k;
			worry(que, k);
		} else if (op == "QUIET") {
			cin >> k;
			quiet(que, k);
		} else if (op == "WORRY_COUNT") {
			worry_count(que);
		}
	}

	return 0;
}
