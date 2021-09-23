/*
В этой задаче вам необходимо вычислить различные демографические показатели для группы людей. Человек представляется структурой Person:
struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};

Тип Gender определён следующим образом:
enum class Gender {
  FEMALE,
  MALE
};

Вам необходимо написать функцию PrintStats, получающую вектор людей, вычисляющую и выводящую медианный возраст для каждой из следующих групп людей:

все люди;

все женщины;

все мужчины;

все занятые женщины;

все безработные женщины;

все занятые мужчины;

все безработные мужчины.

Все 7 чисел нужно вывести в строгом соответствии с форматом (см. пример).
void PrintStats(vector<Person> persons);

Принимая вектор по значению (а не по константной ссылке), вы получаете возможность модифицировать его копию произвольным образом и тем самым проще произвести вычисления.

Подсказка
Используйте алгоритм partition.

Вычисление медианного возраста
Для вычисления медианного возраста группы людей вы должны использовать функцию ComputeMedianAge:
template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end);

Эту функцию не нужно реализовывать самостоятельно: мы реализовали её за вас и автоматически добавим к каждому вашему решению.

Функцию ComputeMedianAge можно вызвать и для пустого набора людей: её результат в этом случае и нужно считать медианным возрастом пустого набора людей.
*/

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

enum class Gender {
    FEMALE,
    MALE
};

struct Person {
    int age;  // возраст
    Gender gender;  // пол
    bool is_employed;  // имеет ли работу
};

template <typename InputIt>
int ComputeMedianAge(InputIt range_begin, InputIt range_end) {
    if (range_begin == range_end) {
        return 0;
    }
    vector<typename InputIt::value_type> range_copy(range_begin, range_end);
    auto middle = begin(range_copy) + range_copy.size() / 2;
    nth_element(
            begin(range_copy), middle, end(range_copy),
            [](const Person& lhs, const Person& rhs) {
                return lhs.age < rhs.age;
            }
    );
    return middle->age;
}

void PrintStats(vector<Person> persons) {
    cout << "Median age = " << ComputeMedianAge(begin(persons), end(persons))
         << endl;
    cout << "Median age for females = " <<
         ComputeMedianAge(begin(persons), partition(begin(persons), end(persons),
                 [](Person p) {
                     return p.gender == Gender::FEMALE;
                 })) << endl;
    cout << "Median age for males = " <<
         ComputeMedianAge(begin(persons), partition(begin(persons), end(persons),
                 [](Person p) {
                     return p.gender == Gender::MALE;
                 })) << endl;
    cout << "Median age for employed females = " <<
         ComputeMedianAge(begin(persons), partition(begin(persons), end(persons),
                 [](Person p) {
                     return p.gender == Gender::FEMALE && p.is_employed == true;
                 })) << endl;
    cout << "Median age for unemployed females = " <<
         ComputeMedianAge(begin(persons), partition(begin(persons), end(persons),
                 [](Person p) {
                     return p.gender == Gender::FEMALE && p.is_employed == false;
                 })) << endl;
    cout << "Median age for employed males = " <<
         ComputeMedianAge(begin(persons), partition(begin(persons), end(persons),
                 [](Person p) {
                     return p.gender == Gender::MALE && p.is_employed == true;
                 })) << endl;
    cout << "Median age for unemployed males = " <<
         ComputeMedianAge(begin(persons), partition(begin(persons), end(persons),
                 [](Person p) {
                     return p.gender == Gender::MALE && p.is_employed == false;
                 })) << endl;
}

int main() {
    vector<Person> persons = {
            {31, Gender::MALE, false},
            {40, Gender::FEMALE, true},
            {24, Gender::MALE, true},
            {20, Gender::FEMALE, true},
            {80, Gender::FEMALE, false},
            {78, Gender::MALE, false},
            {10, Gender::FEMALE, false},
            {55, Gender::MALE, true},
    };
    PrintStats(persons);
    return 0;
}
