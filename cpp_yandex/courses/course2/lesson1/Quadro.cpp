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

