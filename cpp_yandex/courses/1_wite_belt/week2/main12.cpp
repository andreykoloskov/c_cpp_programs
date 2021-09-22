/*
���������� ���������� ������ �����.

�� ���� ��������� ��������� ��������� �������:

CHANGE_CAPITAL country new_capital � ��������� ������� ������ country �� new_capital, ���� ���������� ����� ������ � ����� ��������, ���� ������ � �� ����.

RENAME old_country_name new_country_name � �������������� ������ �� old_country_name � new_country_name.

ABOUT country � ����� ������� ������ country.

DUMP � ����� ������ ���� �����.

������ �����
� ������ ������ ���������� ���������� �������� Q, � ��������� Q ������� � �������� ��������. ��� �������� ����� � ������ ������� ���� �� ��������� ����, ���� � �������� �������������.

������ ������
�������� ��������� ��������� ������� �������:

� ����� �� ������ CHANGE_CAPITAL country new_capital ��������
Introduce new country country with capital new_capital, ���� ������ country ������ �� ������������;

Country country hasn't changed its capital, ���� ������ country �� �������� ������� ����� ������� new_capital;

Country country has changed its capital from old_capital to new_capital, ���� ������ country �� �������� ������� ����� ������� old_capital, �������� ������� �� ��������� � ��������� new_capital.

� ����� �� ������ RENAME old_country_name new_country_name ��������
Incorrect rename, skip, ���� ����� �������� ������ ��������� �� ������, ������ old_country_name �� ���������� ��� ������ new_country_name ��� ����������;

Country old_country_name with capital capital has been renamed to new_country_name, ���� ������ ��������� � ������ ����� ������� capital.

� ����� �� ������ ABOUT country ��������
Country country doesn't exist, ���� ������ � ��������� country �� ����������;

Country country has capital capital, ���� ������ country ���������� � ����� ������� capital.

� ����� �� ������ DUMP ��������
There are no countries in the world, ���� ���� �� ���� ��������� �� ����� ������;

������������������ ��� ���� country/capital, ����������� ������� ���� �����, ���� � ���� ��� ���� ���� �� ���� ������. ��� ������ ������������������ ���� ���������� ���� ���������� ����������� �� �������� ������ � ��������� ����� ����� ��������.

������ 1
����
6
CHANGE_CAPITAL RussianEmpire Petrograd
RENAME RussianEmpire RussianRepublic
ABOUT RussianRepublic
RENAME RussianRepublic USSR
CHANGE_CAPITAL USSR Moscow
DUMP

�����
Introduce new country RussianEmpire with capital Petrograd
Country RussianEmpire with capital Petrograd has been renamed to RussianRepublic
Country RussianRepublic has capital Petrograd
Country RussianRepublic with capital Petrograd has been renamed to USSR
Country USSR has changed its capital from Petrograd to Moscow
USSR/Moscow

������ 2
����
24
RENAME FakeCountry FarFarAway
ABOUT FarFarAway
DUMP
CHANGE_CAPITAL TsardomOfRussia Moscow
CHANGE_CAPITAL TsardomOfRussia Moscow
CHANGE_CAPITAL ColonialBrazil Salvador
CHANGE_CAPITAL TsardomOfRussia SaintPetersburg
RENAME TsardomOfRussia RussianEmpire
CHANGE_CAPITAL RussianEmpire Moscow
CHANGE_CAPITAL RussianEmpire SaintPetersburg
CHANGE_CAPITAL ColonialBrazil RioDeJaneiro
DUMP
RENAME ColonialBrazil EmpireOfBrazil
ABOUT RussianEmpire
RENAME EmpireOfBrazil UnitedStatesOfBrazil
CHANGE_CAPITAL RussianEmpire Petrograd
RENAME RussianEmpire RussianRepublic
RENAME RussianRepublic USSR
CHANGE_CAPITAL USSR Moscow
CHANGE_CAPITAL UnitedStatesOfBrazil Brasilia
RENAME UnitedStatesOfBrazil FederativeRepublicOfBrazil
ABOUT RussianEmpire
DUMP
RENAME USSR USSR

�����
Incorrect rename, skip
Country FarFarAway doesn't exist
There are no countries in the world
Introduce new country TsardomOfRussia with capital Moscow
Country TsardomOfRussia hasn't changed its capital
Introduce new country ColonialBrazil with capital Salvador
Country TsardomOfRussia has changed its capital from Moscow to SaintPetersburg
Country TsardomOfRussia with capital SaintPetersburg has been renamed to RussianEmpire
Country RussianEmpire has changed its capital from SaintPetersburg to Moscow
Country RussianEmpire has changed its capital from Moscow to SaintPetersburg
Country ColonialBrazil has changed its capital from Salvador to RioDeJaneiro
ColonialBrazil/RioDeJaneiro RussianEmpire/SaintPetersburg
Country ColonialBrazil with capital RioDeJaneiro has been renamed to EmpireOfBrazil
Country RussianEmpire has capital SaintPetersburg
Country EmpireOfBrazil with capital RioDeJaneiro has been renamed to UnitedStatesOfBrazil
Country RussianEmpire has changed its capital from SaintPetersburg to Petrograd
Country RussianEmpire with capital Petrograd has been renamed to RussianRepublic
Country RussianRepublic with capital Petrograd has been renamed to USSR
Country USSR has changed its capital from Petrograd to Moscow
Country UnitedStatesOfBrazil has changed its capital from RioDeJaneiro to Brasilia
Country UnitedStatesOfBrazil with capital Brasilia has been renamed to FederativeRepublicOfBrazil
Country RussianEmpire doesn't exist
FederativeRepublicOfBrazil/Brasilia USSR/Moscow
Incorrect rename, skip
*/
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
