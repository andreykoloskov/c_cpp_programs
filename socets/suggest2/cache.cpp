#include "suggest.h"

#define AGE 60

struct Value
{
	std::string str;
	int age;
};

std::map<std::string, struct Value> cache;

Cache::Cache() {}

int Cache::read_cache(std::string &str0, std::string &str1)
{
	auto search = cache.find(str0);
	if (search != cache.end()) {
		str1 = search->second.str;
		search->second.age = AGE;
		aging_cache();
		return 1;
	}
	aging_cache();
	return 0;
}

void Cache::write_cache(std::string str0, std::string str1)
{
	struct Value value;
	value.str = str1;
	value.age = AGE;
	cache.insert(std::make_pair(str0, value));
	aging_cache();
}

void Cache::aging_cache()
{
	std::map<std::string, struct Value>::iterator p, q;
	p = cache.begin();
	while (p != cache.end()) {
		p->second.age--;
		if (!p->second.age) {
			q = p;
			p++;
			cache.erase(q);
		} else
			p++;
	}
}

void Cache::print_cache()
{
	std::map<std::string, struct Value>::iterator p;
	for (p = cache.begin(); p != cache.end(); p++)
		std::cout << p->first << " : " << std::endl << p->second.str << std::endl;
}
