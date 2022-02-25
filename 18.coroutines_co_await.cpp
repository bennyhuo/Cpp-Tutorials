//
// Created by benny on 2022/1/31.
//
#define __cpp_lib_coroutine
#include <coroutine>
#include <exception>
#include <iostream>

template<typename T>
struct Generator {
  struct promise_type;

  struct SuspendWithValue {
    promise_type *promise;

    _NODISCARD constexpr bool await_ready() const noexcept {
      return false;
    }

    constexpr void await_suspend(std::coroutine_handle<>) const noexcept {}
    T await_resume() const noexcept {
      return promise->value;
    }
  };

  struct promise_type {
    T value;

    std::suspend_always initial_suspend() { return {}; };

    std::suspend_never final_suspend() noexcept { return {}; }

    SuspendWithValue yield_value(T t) {
      value = t;
      return {.promise=this};
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

  T next(T value) {
    handle.promise().value = value;
    handle.resume();
    return handle.promise().value;
  }
};

template<typename T>
Generator<T> sequence() {
  int i = 0;
  while (true) {
    int j = co_yield i;
    i = j + 5;
    // co_await promise->yield_value(i);
  }
}

int main() {
  auto gen = sequence<int64_t>();
  for (int i = 0; i < 10; ++i) {
    std::cout << gen.next(i * 10) << std::endl;
  }

  return 0;
}