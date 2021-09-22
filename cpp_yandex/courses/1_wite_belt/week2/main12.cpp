/*
–еализуйте справочник столиц стран.

Ќа вход программе поступают следующие запросы:

CHANGE_CAPITAL country new_capital Ч изменение столицы страны country на new_capital, либо добавление такой страны с такой столицей, если раньше еЄ не было.

RENAME old_country_name new_country_name Ч переименование страны из old_country_name в new_country_name.

ABOUT country Ч вывод столицы страны country.

DUMP Ч вывод столиц всех стран.

‘ормат ввода
¬ первой строке содержитс€ количество запросов Q, в следующих Q строках Ч описани€ запросов. ¬се названи€ стран и столиц состо€т лишь из латинских букв, цифр и символов подчЄркивани€.

‘ормат вывода
¬ыведите результат обработки каждого запроса:

¬ ответ на запрос CHANGE_CAPITAL country new_capital выведите
Introduce new country country with capital new_capital, если страны country раньше не существовало;

Country country hasn't changed its capital, если страна country до текущего момента имела столицу new_capital;

Country country has changed its capital from old_capital to new_capital, если страна country до текущего момента имела столицу old_capital, название которой не совпадает с названием new_capital.

¬ ответ на запрос RENAME old_country_name new_country_name выведите
Incorrect rename, skip, если новое название страны совпадает со старым, страна old_country_name не существует или страна new_country_name уже существует;

Country old_country_name with capital capital has been renamed to new_country_name, если запрос корректен и страна имеет столицу capital.

¬ ответ на запрос ABOUT country выведите
Country country doesn't exist, если страны с названием country не существует;

Country country has capital capital, если страна country существует и имеет столицу capital.

¬ ответ на запрос DUMP выведите
There are no countries in the world, если пока не было добавлено ни одной страны;

последовательность пар вида country/capital, описывающую столицы всех стран, если в мире уже есть хот€ бы одна страна. ѕри выводе последовательности пары указанного вида необходимо упор€дочить по названию страны и раздел€ть между собой пробелом.

ѕример 1
¬вод
6
CHANGE_CAPITAL RussianEmpire Petrograd
RENAME RussianEmpire RussianRepublic
ABOUT RussianRepublic
RENAME RussianRepublic USSR
CHANGE_CAPITAL USSR Moscow
DUMP

¬ывод
Introduce new country RussianEmpire with capital Petrograd
Country RussianEmpire with capital Petrograd has been renamed to RussianRepublic
Country RussianRepublic has capital Petrograd
Country RussianRepublic with capital Petrograd has been renamed to USSR
Country USSR has changed its capital from Petrograd to Moscow
USSR/Moscow

ѕример 2
¬вод
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

¬ывод
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
