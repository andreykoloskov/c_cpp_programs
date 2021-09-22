/*
�������� �������, �������

���������� PalindromFilter

���������� vector<string>

��������� vector<string> words � int minLength � ���������� ��� ������ �� ������� words, ������� �������� ������������ � ����� ����� �� ������ minLength

������� ������ �������� �� ����� 100 �����, ����� ������ ������ �� ������ 100 ��������.
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

bool IsPalindrom(string& str)
{
	for (int i = 0; i < str.size() / 2; i++)
		if (str[i] != str[str.size() - i - 1])
			return false;
	return true;
}

vector<string>
PalindromFilter(vector<string> words, int minLength)
{
	vector<string> words2;
	for (auto w : words)
		if (w.size() >= minLength && IsPalindrom(w))
			words2.push_back(w);

	return words2;
}

#include <boost/algorithm/string.hpp>
int main()
{
	string str;
	int minLength;
	cin >> str >> minLength;
	vector<string> words;
	boost::split(words, str, boost::is_any_of(","), boost::token_compress_on);

	vector<string> words2 = PalindromFilter(words, minLength);
	for (auto w : words2)
		cout << w << " ";

	return 0;
}
