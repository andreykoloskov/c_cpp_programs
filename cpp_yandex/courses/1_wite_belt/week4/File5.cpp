/*
���������� ��������� �������� �� ���������� ������: ���, �������, ����, ����� � ��� ��������. �������� ������ �� ����� ��������, ��������� ��� �� ������� ������ � ����� �� �������� �������� ������ ����. ������ � ������ ������ � ���� ������ ������������ � �������������� ����������� �������.

������ �����

������ ������ �������� ���� ����� ����� N �� 0 �� 10000 � ����� ���������.

����� ���� N �����, ������ �� ������� �������� ��� ������ ������ �� 1 �� 15 �������� � ��� � ������� ���������� ��������, � ��� ����� ����� �� 0 �� 1000000000 � ����, ����� � ��� ��������.

��������� ������ �������� ���� ����� ����� M �� 0 �� 10000 � ����� ��������.

��������� M ����� �������� ������ ������ �� 1 �� 15 �������� � ������, � ����� ����� �� 1 �� 1000000000 � ����� �������� (��������� ���������� � 1).

������ ������

��� ������� ���� name K, ��� K �� 1 �� N, �������� ����� ������ ��� � ������� K-�� ��������.

��� ������� ���� date K, ��� K �� 1 �� N, �������� ����� ����� ����, ����� � ��� �������� K-�� ��������.

��� ��������� �������� �������� bad request.

NB: �������� ������� ����� �������� �� �������, ��������� ��� ������.

������ �����
3
Ivan Ivanov 1 1 1901
Petr Petrox 2 2 1902
Alexander Sidorov 3 3 1903
3
name 1
date 3
mark 5

������ ������
Ivan Ivanov
3.3.1903
bad request
*/

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;

struct Student {
    string first_name;
    string second_name;
    int year;
    int month;
    int day;
};

void PrintStudentData(const vector<Student>& students, string data,
                      int num) {
    if (data == "name" && num > 0 && num <= students.size()) {
        cout << students[num - 1].first_name << " "
                << students[num - 1].second_name << endl;
    } else if (data == "date" && num > 0 && num <= students.size()) {
       cout << students[num - 1].day << "." << students[num - 1].month
               << "." << students[num - 1].year << endl;
    } else {
        cout << "bad request" << endl;
    }
}

int main() {
    int n;
    cin >> n;
    vector<Student> students;

    for (int i = 0; i < n; i++) {
        Student student;
        cin >> student.first_name >> student.second_name
                >> student.day >> student.month >> student.year;
        students.push_back(student);
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; i++) {
        string data;
        int num;
        cin >> data >> num;
        PrintStudentData(students, data, num);
    }

    return 0;
}
