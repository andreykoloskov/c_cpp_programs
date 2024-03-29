/*
��������� ����� Person �� ������ ������ � ������� � 2� �������������, ����������� ������ ��� � ������� �������� ��� ��������, � ����� ��� ��� ��������. ����� �� ������ ����� ������������ �� ���������.

��� ��������� �� ���� ����, ������� ������ ���� ��������:

������ GetFullName � GetFullNameWithHistory ������ �������� "No person";

������ ChangeFirstName � ChangeLastName ������ ������������ ������.

����� ����, ���������� �������� ������������ ��� ������, ������� �� ���� ��� ��������.

������
���
int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}

�����
No person
Polina Sergeeva
Appolinaria (Polina) Sergeeva
Appolinaria (Polina) Ivanova (Sergeeva)
*/

#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

using namespace std;

vector<string> FindNamesHistory(const map<int, string>& names_by_year,
                                int year) {
  vector<string> names;
  for (const auto& item : names_by_year) {
    if (item.first <= year && (names.empty() || names.back() != item.second)) {
      names.push_back(item.second);
    }
  }
  return names;
}

string BuildJoinedName(vector<string> names) {
  if (names.empty()) {
    return "";
  }
  reverse(begin(names), end(names));

  string joined_name = names[0];

  for (int i = 1; i < names.size(); ++i) {
    if (i == 1) {
      joined_name += " (";
    } else {
      joined_name += ", ";
    }
    joined_name += names[i];
  }

  if (names.size() > 1) {
    joined_name += ")";
  }
  return joined_name;
}

string BuildFullName(const string& first_name, const string& last_name) {
  if (first_name.empty() && last_name.empty()) {
    return "Incognito";
  } else if (first_name.empty()) {
    return last_name + " with unknown first name";
  } else if (last_name.empty()) {
    return first_name + " with unknown last name";
  } else {
    return first_name + " " + last_name;
  }
}

class Person {
public:
  Person(string first_name, string last_name, int year) {
    first_names[year] = first_name;
	last_names[year] = last_name;
	year_of_birth = year;
  }

  void ChangeFirstName(int year, const string& first_name) {
	if (year < year_of_birth)
		return;

    first_names[year] = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
	if (year < year_of_birth)
		return;

    last_names[year] = last_name;
  }

  string GetFullName(int year) const {
	if (year < year_of_birth)
		return "No person";

    const vector<string> first_names_history = FindFirstNamesHistory(year);
    const vector<string> last_names_history = FindLastNamesHistory(year);

    string first_name;
    string last_name;
    if (!first_names_history.empty()) {
      first_name = first_names_history.back();
    }
    if (!last_names_history.empty()) {
      last_name = last_names_history.back();
    }
    return BuildFullName(first_name, last_name);
  }

  string GetFullNameWithHistory(int year) const {
	if (year < year_of_birth)
		return "No person";

    const string first_name = BuildJoinedName(FindFirstNamesHistory(year));
    const string last_name = BuildJoinedName(FindLastNamesHistory(year));
    return BuildFullName(first_name, last_name);
  }

private:
  vector<string> FindFirstNamesHistory(int year) const {
    return FindNamesHistory(first_names, year);
  }
  vector<string> FindLastNamesHistory(int year) const {
    return FindNamesHistory(last_names, year);
  }

  map<int, string> first_names;
  map<int, string> last_names;
  int year_of_birth;
};

int main() {
  Person person("Polina", "Sergeeva", 1960);
  for (int year : {1959, 1960}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  person.ChangeFirstName(1965, "Appolinaria");
  person.ChangeLastName(1967, "Ivanova");
  for (int year : {1965, 1967}) {
    cout << person.GetFullNameWithHistory(year) << endl;
  }

  return 0;
}
