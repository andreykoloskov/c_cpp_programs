#include <iostream>
#include <string>
#include <vector>

using namespace std;

void
MoveStrings(vector<string>& src, vector<string>& dst)
{
	for (auto w : src)
		dst.push_back(w);
	src.clear();
}

int main()
{
	vector<string> source = {"a", "b", "c"};
	vector<string> destination = {"z"};
	MoveStrings(source, destination);
	cout << "source" << endl;
	for (auto w : source)
		cout << w << " ";
	cout << endl << "destination" << endl;
	for (auto w : destination)
		cout << w << " ";
	cout << endl;
	return 0;
}
