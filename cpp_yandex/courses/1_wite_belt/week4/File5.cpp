/*
ќпределите структуру Ђ—тудентї со следующими пол€ми: им€, фамили€, день, мес€ц и год рождени€. —оздайте вектор из таких структур, заполните его из входных данных и затем по запросам выведите нужные пол€. „тение и запись данных в этой задаче производитс€ с использованием стандартных потоков.

‘ормат ввода

ѕерва€ строка содержит одно целое число N от 0 до 10000 Ч число студентов.

ƒалее идут N строк, кажда€ из которых содержит две строки длиной от 1 до 15 символов Ч им€ и фамилию очередного студента, и три целых числа от 0 до 1000000000 Ч день, мес€ц и год рождени€.

—ледующа€ строка содержит одно целое число M от 0 до 10000 Ч число запросов.

—ледующие M строк содержат строку длиной от 1 до 15 символов Ч запрос, и целое число от 1 до 1000000000 Ч номер студента (нумераци€ начинаетс€ с 1).

‘ормат вывода

ƒл€ запроса вида name K, где K от 1 до N, выведите через пробел им€ и фамилию K-го студента.

ƒл€ запроса вида date K, где K от 1 до N, выведите через точку день, мес€ц и год рождени€ K-го студента.

ƒл€ остальных запросов выведите bad request.

NB: неверные запросы могут выходить за границы, указанные дл€ данных.

ѕример ввода
3
Ivan Ivanov 1 1 1901
Petr Petrox 2 2 1902
Alexander Sidorov 3 3 1903
3
name 1
date 3
mark 5

ѕример вывода
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
