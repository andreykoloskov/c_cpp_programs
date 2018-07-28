#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

int
main()
{
	int q;
	cin >> q;
	map<vector<string>, int> bus;

	for (int i = 0; i < q; i++) {
		int n;
		cin >> n;
		vector<string> stops(n);
		for (auto& stop : stops)
			cin >> stop;

		if (bus.count(stops) == 1) {
			cout << "Already exists for " << bus[stops] << endl;
		} else {
			int num = bus.size() + 1;
			bus[stops] = num;
			cout << "New bus " << num << endl;
		}
	}

	return 0;
}
