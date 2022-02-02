//
// Created by benny on 2022/1/31.
//
#define __cpp_lib_coroutine
#include <coroutine>
#include <exception>
#include <iostream>

template<typename T>
struct Generator {
  struct promise_type {
    T value;

    std::suspend_always initial_suspend() { return {}; };

    std::suspend_never final_suspend() noexcept { return {}; }

    std::suspend_always yield_value(T t) {
      value = t;
      return {};
    }

    void unhandled_exception() {

    }

    Generator<T> get_return_object() {
      return Generator<T>{ std::coroutine_handle<promise_type>::from_promise(*this) };
    }

    void return_void() {

    }
  };

  std::coroutine_handle<promise_type> handle;

  T next() {
    handle.resume();
    return handle.promise().value;
  }
};

template<typename T>
Generator<T> sequence() {
  int i = 0;
  while (true) {
    co_yield i++;
    // co_await promise->yield_value(i);
  }
}

int main() {
  auto gen = sequence<int64_t>();
  for (int i = 0; i < 10; ++i) {
    std::cout << gen.next() << std::endl;
  }

  auto gen2 = []() -> Generator<int64_t > {
    co_yield 0;
    co_yield 1;

    int a = 0;
    int b = 1;
    while(true) {
      co_yield a + b;
      b = a + b;
      a = b - a;
    }
  }();

  for (int i = 0; i < 10; ++i) {
    std::cout << gen2.next() << std::endl;
  }
  return 0;
}