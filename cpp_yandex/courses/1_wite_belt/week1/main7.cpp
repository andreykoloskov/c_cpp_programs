/*
ƒана строка. Ќайдите в этой строке второе вхождение буквы f и выведите  индекс этого вхождени€. ≈сли буква f в данной строке встречаетс€ только  один раз, выведите число -1, а если не встречаетс€ ни разу, выведите  число -2. »ндексы нумеруютс€ с нул€.
*/

#include <iostream>
#include <string>

using namespace std;

int main()
{
	string str;
	cin >> str;
	int i, cnt = 0;
	for (i = 0; i < str.size(); i++) {
		if (str[i] == 'f')
			cnt++;
		if (cnt == 2)
			break;
	}

	if (cnt == 0)
		cout << -2 << endl;
	else if (cnt == 1)
		cout << -1 << endl;
	else if (cnt == 2)
		cout << i << endl;

	return 0;
}
