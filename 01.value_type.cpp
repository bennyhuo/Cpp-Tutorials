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

  // region reference
  Person &personRef = person;
  person.age = 20;
  cout << "Ref: " << person << endl;
  cout << "Ref: " << personRef << endl;
  // endregion

  // region pointer

  Person *personPtr3 = &person;
  Person *personPtr = new Person("benny", 10);
  Person *personPtr2 = personPtr;
  personPtr->age = 11;
  cout << "Pointer: " << *personPtr << endl;
  cout << "Pointer: " << *personPtr2 << endl;

  delete personPtr;

  // delete personPtr2; // free(): double free detected in tcache 2
  // endregion

  return 0;
}
