#include <iostream>
#include <span>

using namespace std;

template <typename T>
void Print(span<T> span_arr) {
  for (auto a : span_arr) {
    cout << a << endl;
  }
}

int main(int argc, char** argv) {
  int arr[] = { 1, 2, 3, 4 , 5 };
  Print<int>(arr);

  return 0;
}