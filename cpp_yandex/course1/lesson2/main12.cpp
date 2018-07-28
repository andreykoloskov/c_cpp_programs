#include <iostream>
#include <string>
#include <map>

using namespace std;

void
change_capital(map<string, string>& capitals, const string& country,
		const string& new_capital)
{
	if (capitals.count(country) == 0) {
		capitals[country] = new_capital;
		cout << "Introduce new country " << country << " with capital "
				<< new_capital << endl;
	} else {
		if (capitals[country] == new_capital) {
			cout << "Country " << country << " hasn't changed its capital"
					<< endl;
		} else {
			cout << "Country " << country << " has changed its capital from "
					<< capitals[country] << " to " << new_capital << endl;
			capitals[country] = new_capital;
		}
	}
}

void
rename(map<string, string>& capitals, string old_country_name,
		string new_country_name)
{
	if (old_country_name == new_country_name
			|| capitals.count(old_country_name) == 0
			|| capitals.count(new_country_name) > 0) {
		cout << "Incorrect rename, skip" << endl;
	} else {
		string capital = capitals[old_country_name];
		cout << "Country " << old_country_name << " with capital "
				<< capital << " has been renamed to " << new_country_name
				<< endl;
		capitals.erase(old_country_name);
		capitals[new_country_name] = capital;
	}

}

void
about(map<string, string>& capitals, string country)
{
	if (capitals.count(country) == 0) {
		cout << "Country " << country << " doesn't exist" << endl;
	} else {
		cout << "Country " << country << " has capital " << capitals[country] << endl;
	}
}

void
dump(const map<string, string>& capitals)
{
	if (capitals.size() == 0) {
		cout << "There are no countries in the world" << endl;
	} else {
		for (const auto& c : capitals)
			cout << c.first << "/" << c.second << " ";
		cout << endl;
	}
}

int
main()
{
	map<string, string> capitals;
	int q;
	cin >> q;
	for (int i = 0; i < q; i++) {
		string op;
		cin >> op;
		if (op == "CHANGE_CAPITAL") {
			string country, new_capital;
			cin >> country >> new_capital;
			change_capital(capitals, country, new_capital);
		} else if (op == "RENAME") {
			string old_country_name, new_country_name;
			cin >> old_country_name >> new_country_name;
			rename(capitals, old_country_name, new_country_name);
		} else if (op == "ABOUT") {
			string country;
			cin >> country;
			about(capitals, country);
		} else if (op == "DUMP") {
			dump(capitals);
		}
	}

	return 0;
}
