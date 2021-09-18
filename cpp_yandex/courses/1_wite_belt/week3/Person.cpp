#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;

string
FindName(const map<int, string>& names, int year)
{
	string name;
	for (const auto& item : names) {
		if (item.first <= year)
			name = item.second;
		else
			break;
	}
	return name;
}

vector<string>
FindHistoryName(const map<int, string>& names, int year)
{
	vector<string> history_names;
	for (const auto& item : names) {
		if (item.first <= year && (history_names.empty()
				|| history_names.back() != item.second))
			history_names.push_back(item.second);
	}

	return history_names;
}

string
BuildName(vector<string> history_names)
{
	if (history_names.empty())
		return "";

	reverse(begin(history_names), end(history_names));
	string name = history_names[0];
	for (int i = 1; i < history_names.size(); i++) {
		if (i == 1)
			name += " (";
		else
			name += ", ";

		name += history_names[i];
	}

	if (history_names.size() > 1)
		name += ")";

	return name;
}

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
	  first_names[year] = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
	  last_names[year] = last_name;
  }

  string BuildFullName(string first_name, string last_name, int year)
  {
	  if (first_name.empty() && last_name.empty())
		  return "Incognito";

	  if (first_name.empty())
		  return last_name + " with unknown first name";

	  if (last_name.empty())
		  return first_name + " with unknown last name";

	  return first_name + " " + last_name;
  }

  string GetFullName(int year) {
	  const string first_name = FindName(first_names, year);
	  const string last_name = FindName(last_names, year);

	  return BuildFullName(first_name, last_name, year);
  }
  string GetFullNameWithHistory(int year) {
	  string first_name = BuildName(FindHistoryName(first_names, year));
	  string last_name = BuildName(FindHistoryName(last_names, year));

	  return BuildFullName(first_name, last_name, year);
   }
private:
  map<int, string> first_names;
  map<int, string> last_names;
};

int
main()
{
	Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");
	for (int year : {1900, 1965, 1990}) {
	    cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
	    cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
	    cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeFirstName(1990, "Polina");
	person.ChangeLastName(1990, "Volkova-Sergeeva");
	cout << person.GetFullNameWithHistory(1990) << endl;

	person.ChangeFirstName(1966, "Pauline");
	cout << person.GetFullNameWithHistory(1966) << endl;

	person.ChangeLastName(1960, "Sergeeva");
	for (int year : {1960, 1967}) {
	    cout << person.GetFullNameWithHistory(year) << endl;
	}

	person.ChangeLastName(1961, "Ivanova");
	cout << person.GetFullNameWithHistory(1967) << endl;

	return 0;
}
