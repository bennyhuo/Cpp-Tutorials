#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Buffer {
 private:
  unsigned char *buf;
  int capacity;
  int length;
 public:
  explicit Buffer(int capacity) : capacity(capacity), length(0) {
    buf = capacity == 0 ? nullptr : new unsigned char[capacity]{};
    cout << "Constructor: " << *this << endl;
  }

  //region Copy Construct&Assign
  Buffer(Buffer &buffer) : Buffer(0) {
    *this = buffer;
  }

  Buffer &operator=(Buffer const &buffer) {
    cout << "Copy: " << buffer << endl;
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

  //region Move Construct&Assign
  Buffer(Buffer &&buffer)  noexcept : Buffer(0) {
//    *this = std::move(buffer);
    *this = static_cast<Buffer&&>(buffer);
  }

  Buffer &operator=(Buffer &&buffer)  noexcept {
    cout << "Move: " << buffer << endl;
    if (this != &buffer) {
      this->capacity = buffer.capacity;
      this->length = buffer.length;
      delete[] this->buf;
      this->buf = buffer.buf;

      buffer.capacity = 0;
      buffer.length = 0;
      buffer.buf = nullptr;
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
    cout << "Delete: " << *this << endl;
    delete[] buf;
  }

  friend ostream &operator<<(ostream &os, Buffer const &buffer);
};

ostream &operator<<(ostream &os, Buffer const &buffer) {
  os << "Buffer(" << buffer.length << "/" << buffer.capacity << ")[";
  for (int i = 0; i < buffer.capacity; i++) {
    os << (int) buffer.buf[i] << ", ";
  }
  os << "]";
  return os;
}

int main() {
  auto buffer = Buffer(10);
  buffer = Buffer(16);
  cout << buffer << endl;
  auto buffer2 = buffer;
  cout << "------------------" << endl;

  auto buffers = vector<Buffer>();
  buffers.push_back(Buffer(12));
  buffers.push_back(Buffer(20));
  buffers.push_back(Buffer(30));
  buffers.push_back(Buffer(40));
  buffers.push_back(Buffer(50));
  buffers.push_back(Buffer(60));
  return 0;
}
