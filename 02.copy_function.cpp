#include <iostream>

using namespace std;

class Person {
 public:
  string name;
  int age;

  Person(char const *name, int age) : name(name), age(age) {}

  Person(Person &other) = default;

};

ostream &operator<<(ostream &os, Person const &person) {
  return os << "Person(" << person.name << ", " << person.age << ")";
}

int main() {
  // region value
  Person person = Person("benny", 10);
  Person person2 = person;
  person.age = 11;
  cout << "Value: " << person << endl;
  cout << "Value: " << person2 << endl;
  // endregion

  return 0;
}
