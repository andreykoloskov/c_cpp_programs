/*
Реализуйте шаблонную функцию Sqr, которая работает не только для чисел, но и для контейнеров. Функция должна возвращать копию исходного контейнера, модифицировав его следующим образом:

для vector элементы нужно возвести в квадрат;

для map в квадрат нужно возвести только значения, но не ключи;

для pair в квадрат нужно возвести каждый элемент пары.

Функция должна корректно работать не только для контейнеров, состоящих из чисел, но и для составных объектов, например, векторов словарей пар чисел.

Для успешной сдачи решения необходимо сделать предварительное объявление шаблонных функций перед всеми шаблонными функциями.
*/

#include <iostream>
#include <vector>
#include <map>
#include <tuple>

using namespace std;

template <typename T> T Sqr(T t);
template <typename T> vector<T> Sqr(const vector<T>& vr);
template <typename T1, typename T2> map<T1, T2> Sqr(const map<T1, T2>& mp);
template <typename T1, typename T2> pair<T1, T2> Sqr(const pair<T1, T2>& pr);

template <typename T>
T Sqr(T t) {
    return t * t;
}

template <typename T>
vector<T> Sqr(const vector<T>& vr) {
    vector<T> vr2;
    for (auto& v : vr)
        vr2.push_back(Sqr(v));
    return vr2;
}

template <typename T1, typename T2>
map<T1, T2> Sqr(const map<T1, T2>& mp) {
    map<T1, T2> mp2;
    for (auto& m : mp)
        mp2[m.first] = Sqr(m.second);
    return mp2;
}

template <typename T1, typename T2>
pair<T1, T2> Sqr(const pair<T1, T2>& pr) {
    return make_pair(Sqr(pr.first), Sqr(pr.second));
}

int main()
{
	return 0;
}

