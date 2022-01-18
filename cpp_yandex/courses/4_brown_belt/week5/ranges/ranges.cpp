#include <iostream>
#include <vector>
#include <algorithm>
#include <ranges>

//namespace rng = std::ranges;
using namespace std;

//template <rng::input_range Range>
template <typename Range>
void Print(const Range& range) {
  for (const auto& x : range) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
}

int main() {
  std::vector v = {4, 1, 7, 2, 3, 8};
  //rng::sort(v);
  sort(v.begin(), v.end());
  Print(v);
  
  return 0;
}