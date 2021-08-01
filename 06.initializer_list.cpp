#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Data {
 public:
  int a;
  int b;
};

class Person {
 public:
  string name;
  int age;

  Person(char const *name, int age) : name(name), age(age) {}

  Person(Person &other) = default;

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

template<typename T>
void Println(std::initializer_list<T> args) {
  for (auto arg: args) {
    cout << arg << ", ";
  }
  cout << endl;
}

template<typename T>
class PrintlnClass {
 public:
  PrintlnClass(std::initializer_list<T> args) {
    for (auto arg: args) {
      cout << arg << ", ";
    }
    cout << endl;
  }
};

int main() {
  auto person = Person("benny", 30.0);
  auto person2 = Person{"benny", 30};
  Person person3{person};
  Person person4{"benny;30"};

  string name = "benny";
  cout << "Hello " << name << endl;

  vector<int> list = {1, 2, 3, 4, 5};
  for (auto item: list) {
    cout << item << endl;
  }

  Println({1, 2, 3, 4, 5});
  Println({1.0, 2.0, 3.0, 4.0, 5.0});
  auto println_class = PrintlnClass{1.0, 2.0, 3.0, 4.0, 5.0};

  map<string, string> map = {
      {"name", "benny"},
      {"age", "30"},
      {"company", "yfd"}
  };

  for (const auto &item: map) {
    cout << item.first << ":" << item.second << endl;
  }
  return 0;
}
