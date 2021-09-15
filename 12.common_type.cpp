//
// Created by benny on 2021/8/28.
//

#include <utility>
#include <string>
#include <typeinfo>
#include <iostream>
using namespace std;

class A {

};

class B {

 public:

  B() {}

  B(A *a) {

  }

  B(B const &b) {

  }
};

template<typename ... U>
auto GetNth(int n, U ... u) {
  int i = 0;
  using CommonType = common_type_t<U...>;
  CommonType result;
  ((i++ == n && (result = u, true)) || ...);
  return result;
}

int main() {
  using CommonType = common_type_t<A*, B>;
  CommonType a;

  cout << typeid(a).name() << endl;
  return 0;
}