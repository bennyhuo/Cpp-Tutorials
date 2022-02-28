//
// Created by benny on 2022/2/1.
//
#define __cpp_lib_coroutine

#include <chrono>
#include <coroutine>
#include <exception>
#include <future>
#include <iostream>
#include <thread>
#include <type_traits>

using namespace std;

template<typename ... U>
void Println(U... u) {
  int i = 0;
  auto printer = [&i]<typename Arg>(Arg arg) {
    if (sizeof...(U) == ++i) cout << arg << endl;
    else cout << arg << ", ";
  };
  (printer(u), ...);
}

template<typename T, typename... Args> requires (!std::is_void_v<T> && !std::is_reference_v<T>)
struct std::coroutine_traits<std::future<T>, Args...> {
  struct promise_type : std::promise<T> {
    std::future<T> get_return_object() noexcept {
      return this->get_future();
    }

    std::suspend_never initial_suspend() const noexcept {
      return {};
    }
    std::suspend_never final_suspend() const noexcept { return {}; }

    void return_value(const T &value)
    noexcept(std::is_nothrow_copy_constructible_v<T>) {
      this->set_value(value);
    }
    void return_value(T &&value)
    noexcept(std::is_nothrow_move_constructible_v<T>) {
      this->set_value(std::move(value));
    }
    void unhandled_exception() noexcept {
      this->set_exception(std::current_exception());
    }

    std::future<T> await_transform(T(*f)()) {
      return std::async(launch::deferred, [=](){
        auto t = f();

        return t;
      });
    }

    std::future<T> await_transform(std::future<T>(*f)()) {
      return f();
    }
  };
};

// Same for std::future<void>.
template<typename... Args>
struct std::coroutine_traits<std::future<void>, Args...> {
  struct promise_type : std::promise<void> {
    std::future<void> get_return_object() noexcept {
      return this->get_future();
    }

    std::suspend_never initial_suspend() const noexcept { return {}; }
    std::suspend_never final_suspend() const noexcept { return {}; }

    void return_void() noexcept {
      this->set_value();
    }
    void unhandled_exception() noexcept {
      this->set_exception(std::current_exception());
    }
  };
};

// Allow co_await'ing std::future<T> and std::future<void>
// by naively spawning a new thread for each co_await.
template<typename T>
auto operator co_await(std::future<T> future) noexcept requires (!std::is_reference_v<T>) {

struct awaiter : std::future<T> {
  bool await_ready() const noexcept {
    using namespace std::chrono_literals;
    return this->wait_for(0s) != std::future_status::timeout;
  }
  void await_suspend(std::coroutine_handle<> handle) const {
    std::thread([this, handle] {
      Println(this_thread::get_id(), "await");
      this->wait();
      handle.resume();
    }).detach();
  }
  T await_resume() { return this->get(); }
};
return awaiter{
std::move(future)
};
}

// Utilize the infrastructure we have established.
std::future<int> compute() {
  int a = co_await [] {
    Println(this_thread::get_id(), "a");
    return 7;
  };

  int c = co_await []() -> future<int> {
    Println(this_thread::get_id(), "c");
    co_return 10;
  };

  int b = co_await [] {
    Println(this_thread::get_id(), "b");
    return 8;
  };
  co_return a * b * c;
}

int HardWork(int param) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(10000ms);
  return param + 10;
}

int main() {
  Println(this_thread::get_id(), "main");
  Println(compute().get());
  return 0;
}
