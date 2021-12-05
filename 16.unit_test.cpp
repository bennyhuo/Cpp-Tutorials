#define CATCH_CONFIG_MAIN
#include <iostream>
#include <string>
#include <vector>
#include <catch2/catch.hpp>

using std::cout;
using std::endl;
using std::ostream;

class Buffer {
 private:
  unsigned char *buf;
  int capacity;
  int length;
 public:
  explicit Buffer(int capacity) : capacity(capacity), length(0) {
    buf = capacity == 0 ? nullptr : new unsigned char[capacity]{};
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
    std::copy(buffer.buf, buffer.buf + buffer.capacity, buf);
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

  bool operator==(Buffer const &other) const {
    return capacity == other.capacity &&
        length == other.length &&
        std::memcmp(buf, other.buf, length) == 0;
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

  explicit Bitmap(int size) : buffer(size) {}

  friend void swap(Bitmap &lhs, Bitmap &rhs) {
    using std::swap;
    swap(lhs.buffer, rhs.buffer);
  }

};

TEST_CASE("Buffer", "[Buffer]") {
  SECTION("Basic") {
    Buffer buffer(16);
    REQUIRE(buffer.GetLength() == 0);
    REQUIRE(buffer.GetCapacity() == 16);
    buffer.write(1);
    buffer.write(2);
    REQUIRE(buffer.GetLength() == 2);
    REQUIRE(buffer.GetCapacity() == 16);
  }

  SECTION("Copy Constructor") {
    Buffer buffer1(16);
    buffer1.write(1);
    buffer1.write(2);
    Buffer buffer2 = buffer1;
    REQUIRE(buffer2 == buffer1);
  }
}

TEST_CASE("Bitmap", "[Bitmap]") {
  Bitmap bitmap(5);
  REQUIRE(bitmap.buffer.GetCapacity() == 5);
}