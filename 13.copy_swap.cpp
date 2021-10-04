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

  static void Swap(Buffer &lhs, Buffer &rhs) noexcept {
    std::swap(lhs.buf, rhs.buf);
    std::swap(lhs.capacity, rhs.capacity);
    std::swap(lhs.length, rhs.length);
  }

  //region Copy Construct&Assign
  Buffer(Buffer &buffer) :
      capacity(buffer.capacity),
      length(buffer.length),
      buf(buffer.capacity ? new unsigned char[buffer.capacity]{} : nullptr) {
    cout << "Copy Constructor: " << *this << endl;
  }

  Buffer &operator=(Buffer buffer) {
    cout << "Copy: " << buffer << endl;
    Swap(*this, buffer);
    return *this;
  }
  //endregion

  //region Move Construct&Assign
  Buffer(Buffer &&buffer) noexcept: Buffer(0) {
    cout << "Move Constructor: " << *this << endl;
    Swap(*this, buffer);
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
