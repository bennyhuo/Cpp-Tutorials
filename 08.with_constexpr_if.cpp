#include <iostream>
#include <tuple>

using namespace std;

template<typename T, typename ... U>
void Println(T t, U... u) {
  if constexpr(sizeof...(U) == 0) {
    cout << t << endl;
  } else {
    cout << t << ", ";
    Println(u...);
  }
}

constexpr int GetNumber() {
  return 1;
}

int main() {
  tuple<int, double, float, string> tuple = {1, 2.0, 3.0f, "4"};

  Println(1, 2.0, 3.0f, "4");

  Println("hello", 1, "benny", 2, "yfd");

  Println(1, 2, 3, 4, "hello", "bye");

  Println(1);

  int const number = 1;
  int ints[number];
  int ints2[GetNumber()];
  return 0;
}
