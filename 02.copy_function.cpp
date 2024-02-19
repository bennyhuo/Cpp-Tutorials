#include <iostream>
#include <string>

using namespace std;

class Person {
 public:
  string name;
  int age;

  Person(char const *name, int age) : name(name), age(age) {}

  Person(Person const &other) {
    this->name = other.name;
    this->age = 0;
    std::cout << "1111" << endl;
  }

  Person &operator=(Person &other) {
      this->name = other.name;
      this->age = other.age;
    std::cout << "2222" << endl;

    return *this;
  }

  ~Person() {
    cout << "destroy" << endl;
  }
};

Person returnPerson() {
  Person person("benny", 100);
  person.age = 20;
  cout << person.name << endl;
  return person;
}

class Shape {

};

class Circle: public Shape {

};

int main() {
  Shape x = Circle();

  auto p = returnPerson();
  cout << p.name << endl;
  return 0;
}
