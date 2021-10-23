#include "test_runner.h"
#include <string>
#include <string_view>
#include <vector>
#include <set>
#include <map>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target);
  string_view TranslateForward(string_view source) const;
  string_view TranslateBackward(string_view target) const;

private:
    set<string> words; 
    string empty_string = "";
    map<string_view, string_view> dictionary1;
    map<string_view, string_view> dictionary2;
};

void Translator::Add(string_view source, string_view target)
{
    auto it1 = words.find(string(source));
    if (it1 == words.end()) {
        auto res = words.insert(string(source));
        it1 = res.first;
    }

    auto it2 = words.find(string(target));
    if (it2 == words.end()) {
        auto res = words.insert(string(target));
        it2 = res.first;
    }
    dictionary1[*it1] = *it2;
    dictionary2[*it2] = *it1;
}

string_view Translator::TranslateForward(string_view source) const
{
    auto res = dictionary1.find(source);
    return res == dictionary1.end() ? empty_string : res->second;
}

string_view Translator::TranslateBackward(string_view target) const
{
    auto res = dictionary2.find(target);
    return res == dictionary2.end() ? empty_string : res->second;
}

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}