/*
�������� �������
void EnsureEqual(const string& left, const string& right);

� ������, ���� ������ left �� ����� ������ right, ������� ������ ����������� ���������� runtime_error � ����������� "<l> != <r>", ��� <l> � <r> - ������, ������� �������� � ���������� left � right ��������������. �������� ��������, ��� ������ ����� ����������� � ������, ������� ���������� � ����������, ������ ���� ����� �� ������ �������.

���� left == right, ������� ������ ��������� �����������.

��������, ���
int main() {
  try {
    EnsureEqual("C++ White", "C++ White");
    EnsureEqual("C++ White", "C++ Yellow");
  } catch (runtime_error& e) {
    cout << e.what() << endl;
  }
  return 0;
}

������ ��������
C++ White != C++ Yellow
*/

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

void EnsureEqual(const string& left, const string& right) {
    stringstream ss;
    ss << left << " != " << right;
    if (left != right) {
        throw runtime_error(ss.str());
    }
}

int main()
{
	try {
		EnsureEqual("a", "a");
	}
	catch(exception&) {
	}
	
	return 0;
}
