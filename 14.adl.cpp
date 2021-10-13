#include <iostream>
#include <string>
#include <vector>

using std::cout;
//using std::endl;
using std::ostream;

class Buffer {
 private:
  unsigned char *buf;
  int capacity;
  int length;
 public:
  explicit Buffer(int capacity) : capacity(capacity), length(0) {
    buf = capacity == 0 ? nullptr : new unsigned char[capacity]{};
    cout << "Constructor: " << *this << std::endl;
  }

  friend void swap(Buffer &lhs, Buffer &rhs) noexcept {
    using std::swap; // ADL
    swap(lhs.buf, rhs.buf);
    swap(lhs.capacity, rhs.capacity);
    swap(lhs.length, rhs.length);
  }

  //region Copy Construct&Assign
  Buffer(Buffer &buffer) :
      capacity(buffer.capacity),
      length(buffer.length),
      buf(buffer.capacity ? new unsigned char[buffer.capacity]{} : nullptr) {
  }

  Buffer &operator=(Buffer buffer) {
    swap(*this, buffer);
    return *this;
  }
  //endregion

  //region Move Construct&Assign
  Buffer(Buffer &&buffer) noexcept: Buffer(0) {
    swap(*this, buffer);
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

class Bitmap {
 public:
  Buffer buffer;

  explicit Bitmap(int size): buffer(size) { }

  friend void swap(Bitmap &lhs, Bitmap &rhs) {
    using std::swap;
    swap(lhs.buffer, rhs.buffer);
  }

};

int main() {
  auto bitmap = Bitmap(10);
  auto bitmap2 = Bitmap(20);

  using std::swap;
  swap(bitmap, bitmap2);

  endl(cout);
  return 0;
}
