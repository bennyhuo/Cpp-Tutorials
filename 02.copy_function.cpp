#include <iostream>
#include <string>

using namespace std;

class Callback {
 public:
  virtual void OnSuccess() = 0;

  virtual void OnError() = 0;
};

class CallbackImpl: Callback {
 public:
  void OnSuccess() override {
    cout << "success" << endl;
  }
  void OnError() override {
    cout << "error" << endl;
  }
};



class Person {
 public:
  string name;
  int age;

  Person(char const *name, int age) : name(name), age(age) {}

  Person(Person &other) {
    this->name = other.name;
    this->age = 0;
  }

  Person &operator=(Person &other) {
      this->name = other.name;
      this->age = other.age;
      return *this;
  }

  explicit Person(char const *description) {
    string raw_description = description;
    auto index = raw_description.find(';');
    if (index != string::npos) {
      this->name = raw_description.substr(0, index);
      try {
        this->age = stoi(raw_description.substr(index + 1, raw_description.length() - index));
      } catch (...) {
        // ignore any exception.
        this->age = 0;
      }
    } else {
      this->name = "anonymous";
      this->age = 0;
    }
  }
};

ostream &operator<<(ostream &os, Person const &person) {
  return os << "Person(" << person.name << ", " << person.age << ")";
}

int main() {

  Person person("benny", 10);
  Person person2 = person;
  person2 = person;
  person.age = 11;
  cout << "Value: " << person << endl;
  cout << "Value: " << person2 << endl;

  Person person3("bennyhuo;30");
  // person3 = "bennyhuo;40";

  auto callback = CallbackImpl();
  callback.OnSuccess();
  return 0;
}
