#include <iostream>

using namespace std;

int main()
{
	int a, b, x, y;
	cin >> a >> b;
	x = (a >= b) ? a : b;
	y = (a >= b) ? b : a;

	while (x % y != 0) {
		int z = y;
		y = x % y;
		x = z;
	}

	cout << y;

	return  0;
}
