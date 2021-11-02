#include "utilities.h"

void IgnoreSpaces(string_view& str)
{
    while (!str.empty() && isspace(str[0])) {
        str.remove_prefix(1);
    }
}

string_view ReadWord(string_view& str)
{
    IgnoreSpaces(str);
    auto space = str.find(' ');
    auto word = str.substr(0, space);
    str.remove_prefix(space != str.npos ? space : str.size());
    return word;
}

vector<string_view> SplitIntoWords(string_view line)
{
    vector<string_view> result;

    for (auto word = ReadWord(line); word.size() != 0; word = ReadWord(line)) {
        result.push_back(word);
    }
    return result;
}

