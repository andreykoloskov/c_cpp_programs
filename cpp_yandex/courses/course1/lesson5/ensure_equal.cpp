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
