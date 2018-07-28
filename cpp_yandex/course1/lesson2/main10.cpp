// https://gcc.godbolt.org

/*
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int
main()
{
	vector<int> days_in_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	vector<vector<string>> activities(31);
	int month = 0;

	int q;
	cin >> q;
	for (int i = 0; i < q; i++) {
		string op;
		cin >> op;
		if (op == "ADD") {
			int day;
			string s;
			cin >> day >> s;
			activities[day - 1].push_back(s);
		} else if (op == "NEXT") {
			month = (month + 1) % 12;
			int sz = days_in_month[month];
			if (sz < activities.size()) {
				auto& finish_day = activities[sz - 1];
				for (int i = sz; i < activities.size(); i++)
					finish_day.insert(end(finish_day), begin(activities[i]), end(activities[i]));

			}
			activities.resize(sz);
		} else if (op == "DUMP") {
			int day;
			cin >> day;
			auto d =  activities[day - 1];
			cout << d.size() << " ";
			for (auto w : d)
				cout << w << " ";
			cout << endl;
		}
	}

	return 0;
}
*/
