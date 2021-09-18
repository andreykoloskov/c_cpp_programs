#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

int
main()
{
	int q;
	cin >> q;
	map<string, vector<string>> bus_to_stop;
	map<string, vector<string>> stop_to_bus;

	for (int i = 0; i < q; i++) {
		string op;
		cin >> op;
		if (op == "NEW_BUS") {
			string bus;
			int stop_count;
			cin >> bus >> stop_count;
			vector<string> stops(stop_count);

			for (string& stop : stops) {
				cin >> stop;
				stop_to_bus[stop].push_back(bus);
			}

			bus_to_stop[bus] = stops;

		} else if (op == "BUSES_FOR_STOP") {
			string stop;
			cin >> stop;

			if (stop_to_bus.count(stop) == 0) {
				cout << "No stop" << endl;
				continue;
			}

			for (const string& b : stop_to_bus[stop])
				cout << b << " ";
			cout << endl;

		} else if (op == "STOPS_FOR_BUS") {
			string bus;
			cin >> bus;

			if (bus_to_stop.count(bus) == 0) {
				cout << "No bus" << endl;
				continue;
			}

			for (const string& stop : bus_to_stop[bus]) {
				cout << "Stop " << stop << ": ";

				if (stop_to_bus[stop].size() == 1
						&& stop_to_bus[stop][0] == bus) {
					cout << "no interchange" << endl;
					continue;
				}

				for (const string& b : stop_to_bus[stop]) {
					if (b != bus)
						cout << b << " ";
				}
				cout << endl;
			}

		} else if (op == "ALL_BUSES") {
			if (bus_to_stop.size() == 0) {
				cout << "No buses" << endl;
				continue;
			}

			for (auto& b : bus_to_stop) {
				cout << "Bus " << b.first << ": ";

				for (const string& s : b.second) {
					cout << s << " ";
				}
				cout << endl;
			}
		}
	}

	return 0;
}
