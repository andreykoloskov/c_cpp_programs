#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <string_view>
#include <vector>
#include <future>
#include <functional>

using namespace std;

struct Stats {
    map<string, int> word_frequences;

    void operator += (const Stats& other) {
        for (const auto& [word, frequency] : other.word_frequences) {
            word_frequences[word] += frequency;
        }
    }
};

Stats ExploreLine(const set<string>& key_words, const string& line) {
    Stats result;
    string_view ln = line;
    map<string_view, int> parts;
    size_t part_length = 0;

    while ((part_length = ln.find(' ')) != ln.npos)
    {
        parts[string_view(ln.data(), part_length)] += 1;
        ln.remove_prefix(part_length + 1);
    }

    if (!ln.empty())
    {
        parts[string_view(std::move(ln))] += 1;
    }

    for (const auto& key : key_words) {
        auto it = parts.find(key);
        if (it != parts.end()) {
            result.word_frequences[key] += it->second;
        }
    }

    return result;
}

Stats ExploreKeyWordsSingleThread(
  const set<string>& key_words, istream& input
) {
  Stats result;
  for (string line; getline(input, line); ) {
    result += ExploreLine(key_words, line);
  }
  return result;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  // Реализуйте эту функцию
    vector<future<Stats>> futures;
    Stats result;
    int count_in_thread = 50000;
    while (!input.eof()) {
        string words;
        for (int i = 0; i < count_in_thread; ++i) {
            string word;
            input >> word;
            words += move(word) + " ";
        }
        futures.push_back(async(ExploreLine, ref(key_words), words));
    }
    for (auto& f : futures) {
        result += f.get();
    }
    return result;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}