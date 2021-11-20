#pragma once

#include<string>
#include <iostream>
#include <unordered_map>

using namespace std;

namespace Ini {
	
using Section = unordered_map<string, string>;

class Document {
public:
  Document();
  Document(istream& input);
  Section& AddSection(string name);
  const Section& GetSection(const string& name) const;
  size_t SectionCount() const;

private:
  unordered_map<string, Section> sections;
};

Document Load(istream& input);

}