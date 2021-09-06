#include <iostream>
#include <tuple>

using namespace std;

template<typename ... U>
void Println(U... u) {
  int i = 0;
  auto printer = [&i]<typename Arg>(Arg arg) {
    if (sizeof...(U) == ++i) cout << arg << endl;
    else cout << arg << ", ";
  };
  (printer(u), ...);
}

int main() {
  tuple<int, double, float, string> tuple = {1, 2.0, 3.0f, "4"};

  Println(1, 2.0, 3.0f, "4");

  Println("hello", 1, "benny", 2, "yfd");

  Println(1, 2, 3, 4, "hello", "bye");
  return 0;
}
