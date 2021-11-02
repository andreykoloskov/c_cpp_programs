#pragma once

#include <string>
#include <string_view>
#include <vector>

using namespace std;

void IgnoreSpaces(string_view& str);
string_view ReadWord(string_view& str);
vector<string_view> SplitIntoWords(string_view line);