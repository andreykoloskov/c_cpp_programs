#include <algorithm>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include "test_runner.h"

using namespace std;


bool IsSubdomain(string_view subdomain, string_view domain) {
  auto i = 0;
  auto j = 0;
  while (i < subdomain.size() && j < domain.size()) {
    if (subdomain[i++] != domain[j++]) {
      return false;
    }
  }
  return (i >= subdomain.size() && j >= domain.size())
      || (j >= domain.size() && subdomain[i] == '.');
}


vector<string> ReadDomains() {
  size_t count;
  cin >> count;

  vector<string> domains;
  for (size_t i = 0; i < count; ++i) {
    string domain;
    cin >> domain;
    domains.push_back(domain);
  }
  return domains;
}

void PrepareDomains(vector<string>& domains) {
  for (auto& domain : domains) {
    reverse(begin(domain), end(domain));
  }
}

void PrepareBannedDomains(vector<string>& domains) {
  PrepareDomains(domains);
  sort(begin(domains), end(domains));
  size_t insert_pos = 0;
  for (string& domain : domains) {
    if (insert_pos == 0 || !IsSubdomain(domain, domains[insert_pos - 1])) {
      swap(domains[insert_pos++], domain);
    }
  }
  domains.resize(insert_pos);
}

bool DomainIsBanned(const string_view& domain, vector<string>& banned_domains) {
  const auto it = upper_bound(begin(banned_domains), end(banned_domains), domain);
  return it != begin(banned_domains) && IsSubdomain(domain, *prev(it));
}

int main() {
  vector<string> banned_domains = ReadDomains();
  vector<string> domains_to_check = ReadDomains();
  PrepareBannedDomains(banned_domains);
  PrepareDomains(domains_to_check);

  for (const string_view domain : domains_to_check) {
    cout << (DomainIsBanned(domain, banned_domains) ? "Bad" : "Good") << endl;
  }
  return 0;
}