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

//
// Created by benny on 2022/1/31.
//
#define __cpp_lib_coroutine
#include <coroutine>
#include <exception>
#include <iostream>

template<typename ... U>
void Println(U... u) {
  using namespace std;
  int i = 0;
  auto printer = [&i]<typename Arg>(Arg arg) {
    if (sizeof...(U) == ++i) cout << arg << endl;
    else cout << arg << ", ";
  };
  (printer(u), ...);
}

template<typename T>
struct Result {
  struct promise_type {
    T value;

    std::suspend_always initial_suspend() { return {}; };

    std::suspend_never final_suspend() noexcept { return {}; }

    void unhandled_exception() {}

    Result<T> get_return_object() {
      return Result<T>{std::coroutine_handle<promise_type>::from_promise(*this)};
    }

    void return_void() {

    }
  };

  std::coroutine_handle<promise_type> handle;

  T start() {
    handle.resume();
    return handle.promise().value;
  }
};

template<typename T>
struct Awaiter : std::future<T> {
  bool await_ready() const noexcept {
    using namespace std::chrono_literals;
    return this->wait_for(0s) != std::future_status::timeout;
  }
  void await_suspend(std::coroutine_handle<> cont) const {
    std::thread([this, cont] {
      this->wait();
      cont();
    }).detach();
  }
  T await_resume() { return this->get(); }
};

template<typename T>
auto operator
co_await(std::future<T> future) {
  return Awaiter<T>{
    std::move(future)
  };
}

int HardWork(int param) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(10000ms);
  return param + 10;
}

Result<int> sequence() {
  int i = 0;

   auto future = std::async([](){
     Println("async", std::this_thread::get_id());
     using namespace std::chrono_literals;
     std::this_thread::sleep_for(1000ms);
     return 1 + 10;
   });

   auto f = std::move(future);
  //auto future2 = std::async(HardWork, 2);

  co_await std::move(f);

}

int main() {
  Println("main", std::this_thread::get_id());
  auto gen = sequence();

  return 0;
}
