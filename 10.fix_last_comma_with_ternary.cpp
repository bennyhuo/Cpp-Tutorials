#include <iostream>
#include <tuple>

using namespace std;

template<typename ... U>
void Println(U... u) {
  // u0 u1 u2 u3
  // Println(u0, Println(u1, Println(u2, Println(u3))));
  // cout << u0 << ", ", cout << u1 << ", ", ....

  // a ? b : c
  int last_index = sizeof...(u) - 1;
  int i = 0;
  ((i < last_index ? (cout << u << ", ") : (cout << u << endl), i += 1), ...);
  // (i++ < last_index ? (cout << u0 << ", ") : (cout << u0 << endl)), (i++ < last_index ? (cout << u1 << ", ") : (cout << u1 << endl))
}

template<typename ... U>
auto GetNth(int n, U ... u) {
  int i = 0;
  using CommonType = common_type_t<U...>;
  CommonType result;
  ((i++ == n && (result = u, true)) || ...);
  return result;
}

int main() {
  tuple<int, double, float, string> tuple = {1, 2.0, 3.0f, "4"};

  Println(1, 2.0, 3.0f, "4");

  Println("hello", 1, "benny", 2, "yfd");

  Println(1, 2, 3, 4, "hello", "bye");

  auto result = GetNth(4, 1, 2.0f, 3, 4, 5L);
  Println(result);
  return 0;
}
