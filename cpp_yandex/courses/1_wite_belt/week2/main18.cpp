#include <iostream>
#include <string>
#include <set>
#include <map>

using namespace std;

int
main()
{
	int q;
	cin >> q;
	map<set<string>, int> bus;

	for (int i = 0; i < q; i++) {
		int n;
		cin >> n;
		set<string> stops;
		for (int i = 0; i < n; i++) {
			string stop;
			cin >> stop;
			stops.insert(stop);
		}

		if (bus.count(stops) == 1) {
			cout << "Already exists for " << bus[stops] << endl;
		} else {
			int num = (int) bus.size() + 1;
			bus[stops] = num;
			cout << "New bus " << num << endl;
		}
	}

	return 0;
}
