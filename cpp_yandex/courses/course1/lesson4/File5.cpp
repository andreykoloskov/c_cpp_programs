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
