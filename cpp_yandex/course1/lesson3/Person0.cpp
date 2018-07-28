#include <iostream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

class Person {
public:
  void ChangeFirstName(int year, const string& first_name) {
	  first_names[year] = first_name;
  }
  void ChangeLastName(int year, const string& last_name) {
	  last_names[year] = last_name;
  }
  string GetFullName(int year) {
	  string fn0 = "";
	  string ln0 = "";

	  for (auto& fn : first_names) {
		  if (fn.first <= year)
			  fn0 = fn.second;
		  else
			  break;
	  }
	  for (auto& ln : last_names) {
		  if (ln.first <= year)
			  ln0 = ln.second;
		  else
			  break;
	  }

	  if (fn0.length() == 0 && ln0.length() == 0)
		  return "Incognito";
	  if (fn0.length() == 0 && ln0.length() != 0)
		  return ln0 + " with unknown first name";
	  if (fn0.length() != 0 && ln0.length() == 0)
		  return fn0 + " with unknown last name";

	  return fn0 + " " + ln0;
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
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	return 0;
}
