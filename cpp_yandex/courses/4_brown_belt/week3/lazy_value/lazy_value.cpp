#include "test_runner.h"

#include <functional>
#include <string>
#include <optional>
using namespace std;

template <typename T>
class LazyValue {
public:
  explicit LazyValue(std::function<T()> init);

  bool HasValue() const;
  const T& Get() const;

private:
  function<T()> func_;
  mutable optional<T> value_;
};

template<typename T>
LazyValue<T>::LazyValue(std::function<T()> init)
    : func_(init), value_(nullopt) {
}

template<typename T>
bool LazyValue<T>::HasValue() const {
  return value_.has_value();
}

template<typename T>
const T& LazyValue<T>::Get() const {
  if (!value_) {
    value_ = func_();
  }
  return *value_;
}

void UseExample() {
  const string big_string = "Giant amounts of memory";

  LazyValue<string> lazy_string([&big_string] { return big_string; });

  ASSERT(!lazy_string.HasValue());
  ASSERT_EQUAL(lazy_string.Get(), big_string);
  ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
  bool called = false;

  {
    LazyValue<int> lazy_int([&called] {
      called = true;
      return 0;
    });
  }
  ASSERT(!called);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, UseExample);
  RUN_TEST(tr, TestInitializerIsntCalled);
  return 0;
}