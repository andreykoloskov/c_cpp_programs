#include "ini.h"

namespace Ini {
	
Document::Document() {
}

Document::Document(istream& input) {
	string section_key;
	for (string line; getline(input, line); ) {
		if (line[0] == '[' && line[line.size() - 1] == ']') {
			section_key = line.substr(1, line.size() - 2);
			auto it = sections.find(section_key);
			if (it == sections.end()) {
				sections[section_key] = {};
			}
		}
		else if (line.size() != 0) {
			auto pos = line.find("=");
			string key = line.substr(0, pos);
			string value = line.substr(pos + 1);
			sections[section_key][move(key)] = move(value);
		}
	}
}

Section& Document::AddSection(string name) {
  return sections[name];
}

const Section& Document::GetSection(const string& name) const {
	return sections.at(name);
}

size_t Document::SectionCount() const {
	return sections.size();
}

Document Load(istream& input) {
	return Document{ input };
}

}