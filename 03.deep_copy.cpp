#include <iostream>
#include <string>

using namespace std;

class Buffer {
 private:
  unsigned char *buf;
  int capacity;
  int length;
 public:
  explicit Buffer(int capacity) : capacity(capacity), length(0), buf(new unsigned char[capacity]{0}) {

  }

  //region
  Buffer(Buffer &buffer) {
    this->capacity = buffer.capacity;
    this->length = buffer.length;
    this->buf = new unsigned char[buffer.capacity];
    std::copy(buffer.buf, buffer.buf + buffer.capacity, this->buf);
  }

  Buffer &operator=(Buffer const &buffer) {
    if (this != &buffer) {
      this->capacity = buffer.capacity;
      this->length = buffer.length;
      delete[] this->buf;
      this->buf = new unsigned char[buffer.capacity];
      std::copy(buffer.buf, buffer.buf + buffer.capacity, this->buf);
    }

    return *this;
  }
  //endregion

  int GetLength() {
    return length;
  }

  int GetCapacity() {
    return capacity;
  }

  bool write(unsigned char value) {
    if (length == capacity) return false;
    buf[length++] = value;
    return true;
  }

  ~Buffer() {
    delete[] buf;
  }

  friend ostream &operator<<(ostream &os, Buffer &buffer);
};

ostream &operator<<(ostream &os, Buffer &buffer) {
  os << "Buffer(" << buffer.length << "/" << buffer.capacity << ")[";
  for (int i = 0; i < buffer.capacity; i++) {
    os << (int) buffer.buf[i] << ", ";
  }
  os << "]";
  return os;
}

int main() {
  auto buffer = Buffer(10);
  buffer.write(97);
  auto buffer2 = buffer;
  buffer2.write(100);
  auto buffer3 = buffer;
  buffer3 = buffer2;
  buffer3.write(101);
  buffer.write(65);
  cout << buffer << endl;
  cout << buffer2 << endl;
  cout << buffer3 << endl;
  return 0;
}
