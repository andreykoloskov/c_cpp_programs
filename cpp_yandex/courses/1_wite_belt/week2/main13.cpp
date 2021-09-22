/*
���������� ������� �������� ���������� ���������. ��� ����� ������������ ��������� �������:

NEW_BUS bus stop_count stop1 stop2 ... � �������� ������� �������� � ��������� bus � stop_count ����������� � ���������� stop1, stop2, ...

BUSES_FOR_STOP stop � ������� �������� ���� ��������� ��������, ���������� ����� ��������� stop.

STOPS_FOR_BUS bus � ������� �������� ���� ��������� �������� bus �� ������� ���������, �� ������� ����� ��������� �� ������ �� ���������.

ALL_BUSES � ������� ������ ���� ��������� � �����������.

������ �����
� ������ ������ ����� ���������� ���������� �������� Q, ����� � Q ������� ������� �������� ��������.

�������������, ��� ��� �������� ��������� � ��������� ������� ���� �� ��������� ����, ���� � ������ �������������.

��� ������� ������� NEW_BUS bus stop_count stop1 stop2 ... �������������, ��� ������� bus �����������, ���������� ��������� ������ 0, � ����� ����� stop_count ������� ������ ����� ���������� �������� ���������, ������ ��� �������� � ������ ������ ��������.

������ ������
��� ������� �������, ����� NEW_BUS, �������� ��������������� ����� �� ����:

�� ������ BUSES_FOR_STOP stop �������� ����� ������ ������ ���������, ����������� ����� ��� ���������, � ��� �������, � ������� ��� ����������� ��������� NEW_BUS. ���� ��������� stop �� ����������, �������� No stop.

�� ������ STOPS_FOR_BUS bus �������� �������� ��������� �������� bus � ��������� ������� � ��� �������, � ������� ��� ���� ������ � ��������������� ������� NEW_BUS. �������� ������ ��������� stop ������ ����� ��� Stop stop: bus1 bus2 ..., ��� bus1 bus2 ... � ������ ���������, ����������� ����� ��������� stop, � �������, � ������� ��� ����������� ��������� NEW_BUS, �� ����������� ��������� �������� bus. ���� ����� ��������� stop �� ��������� �� ���� �������, ����� bus, ������ ������ ��������� ��� �� �������� no interchange. ���� ������� bus �� ����������, �������� No bus.

�� ������ ALL_BUSES �������� �������� ���� ��������� � ���������� �������. �������� ������� �������� bus ������ ����� ��� Bus bus: stop1 stop2 ..., ��� stop1 stop2 ... � ������ ��������� �������� bus � �������, � ������� ��� ���� ������ � ��������������� ������� NEW_BUS. ���� �������� �����������, �������� No buses.

��������������
������� ������ ���� �������� ����� ������ �������. ���� �� �� ������� � ���, ��� �� �������� �� �����, ����������� ������� ��� ���.

������
����
10
ALL_BUSES
BUSES_FOR_STOP Marushkino
STOPS_FOR_BUS 32K
NEW_BUS 32 3 Tolstopaltsevo Marushkino Vnukovo
NEW_BUS 32K 6 Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
BUSES_FOR_STOP Vnukovo
NEW_BUS 950 6 Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
NEW_BUS 272 4 Vnukovo Moskovsky Rumyantsevo Troparyovo
STOPS_FOR_BUS 272
ALL_BUSES

�����
No buses
No stop
No bus
32 32K
Stop Vnukovo: 32 32K 950
Stop Moskovsky: no interchange
Stop Rumyantsevo: no interchange
Stop Troparyovo: 950
Bus 272: Vnukovo Moskovsky Rumyantsevo Troparyovo
Bus 32: Tolstopaltsevo Marushkino Vnukovo
Bus 32K: Tolstopaltsevo Marushkino Vnukovo Peredelkino Solntsevo Skolkovo
Bus 950: Kokoshkino Marushkino Vnukovo Peredelkino Solntsevo Troparyovo
*/

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
