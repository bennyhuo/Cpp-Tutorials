#include <iostream>
#include <tuple>

using namespace std;

//template<typename T>
//void Println(std::initializer_list<T> args) {
//  for (auto arg: args) {
//    cout << arg << ", ";
//  }
//  cout << endl;
//}
//
//template<typename T>
//class PrintlnClass {
// public:
//  PrintlnClass(std::initializer_list<T> args) {
//    for (auto arg: args) {
//      cout << arg << ", ";
//    }
//    cout << endl;
//  }
//};

template<typename T>
void Println(T t) {
  cout << t << endl;
}

template<typename T, typename ... U>
void Println(T t, U... u) {
  cout << t << ", ";
  Println(u...);
}

int main() {
  tuple<int, double, float, string> tuple = {1, 2.0, 3.0f, "4"};

  Println(1, 2.0, 3.0f, "4");

  Println("hello", 1, "benny", 2, "yfd");

  Println(1,2,3,4, "hello", "bye");
  return 0;
}
