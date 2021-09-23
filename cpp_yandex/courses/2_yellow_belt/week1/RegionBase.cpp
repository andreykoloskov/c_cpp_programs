/*
Имеется база регионов, представленная вектором структур Region:
struct Region {
  string std_name;
  string parent_std_name;
  map<Lang, string> names;
  int64_t population;
};

Здесь Lang — идентификатор языка:
enum class Lang {
  DE, FR, IT
};

Напишите функцию FindMaxRepetitionCount, принимающую базу регионов и определяющую, какое максимальное количество повторов (число вхождений одного и того же элемента) она содержит. Две записи (объекты типа Region) считаются различными, если они отличаются хотя бы одним полем.
int FindMaxRepetitionCount(const vector<Region>& regions);

Если все записи уникальны, считайте максимальное количество повторов равным 1. Если записи отсутствуют, верните 0. Гарантируется, что типа int достаточно для хранения ответа.

Пояснение
В этой задаче вам надо прислать на проверку файл с реализацией функции FindMaxRepetitionCount, а также дополнительных функций, если это необходимо. Этот файл не должен содержать типы Lang и Region. В противном случае вы получите ошибку компиляции.
*/
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <algorithm>

using namespace std;

enum class Lang {
    DE, FR, IT
};

struct Region {
    string std_name;
    string parent_std_name;
    map<Lang, string> names;
    int64_t population;
};

bool operator<(const Region& lhs, const Region& rhs) {
    return tie(lhs.std_name, lhs.parent_std_name, lhs.names, lhs.population)
            < tie(rhs.std_name, rhs.parent_std_name, rhs.names, rhs.population);
}

int FindMaxRepetitionCount(const vector<Region>& regions) {
    if (regions.size() == 0)
        return 0;

    map<Region, int> region_count;
    for (auto r : regions) {
        region_count[r]++;
    }

    int sz = 1;
    for (auto r : region_count) {
        if (r.second > sz)
            sz = r.second;
    }

    return sz;
}

int main() {
    cout << FindMaxRepetitionCount({
                                           {
                                                   "Moscow",
                                                   "Russia",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                   89
                                           }, {
                                                   "Russia",
                                                   "Eurasia",
                                                   {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                                   89
                                           }, {
                                                   "Moscow",
                                                   "Russia",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                   89
                                           }, {
                                                   "Moscow",
                                                   "Russia",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                   89
                                           }, {
                                                   "Russia",
                                                   "Eurasia",
                                                   {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                                   89
                                           },
                                   }) << endl;

    cout << FindMaxRepetitionCount({
                                           {
                                                   "Moscow",
                                                   "Russia",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                   89
                                           }, {
                                                   "Russia",
                                                   "Eurasia",
                                                   {{Lang::DE, "Russland"}, {Lang::FR, "Russie"}, {Lang::IT, "Russia"}},
                                                   89
                                           }, {
                                                   "Moscow",
                                                   "Russia",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou deux"}, {Lang::IT, "Mosca"}},
                                                   89
                                           }, {
                                                   "Moscow",
                                                   "Toulouse",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                   89
                                           }, {
                                                   "Moscow",
                                                   "Russia",
                                                   {{Lang::DE, "Moskau"}, {Lang::FR, "Moscou"}, {Lang::IT, "Mosca"}},
                                                   31
                                           },
                                   }) << endl;

    return 0;
}