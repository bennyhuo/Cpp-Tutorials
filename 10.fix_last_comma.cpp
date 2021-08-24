#include <iostream>
#include <tuple>

using namespace std;

template<typename ... U>
void Println(U... u) {
  // u0 u1 u2 u3
  // Println(u0, Println(u1, Println(u2, Println(u3))));
  // cout << u0 << ", ", cout << u1 << ", ", ....
  size_t index = 0;
  auto printer = [index]<typename Arg>(Arg arg) mutable {
    if (sizeof...(U) == ++index) cout << arg << endl;
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
