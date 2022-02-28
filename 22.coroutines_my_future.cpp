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
  cout << std::chrono::system_clock::now().time_since_epoch().count() / 10000 << ": ";
  int i = 0;
  auto printer = [&i]<typename Arg>(Arg arg) {
    if (sizeof...(U) == ++i) cout << arg << endl;
    else cout << arg << ", ";
  };
  (printer(u), ...);
}

struct Dispatcher {
  _NODISCARD constexpr bool await_ready() const noexcept {
    return false;
  }

  void await_suspend(coroutine_handle<> this_handle) const noexcept {
    std::thread work_thread([this_handle]() {
      this_thread::sleep_for(std::chrono::seconds(2));
      this_handle.resume();
    });
    work_thread.detach();
  }
  constexpr void await_resume() const noexcept {}
};

template<typename T, typename... Args> requires (!std::is_void_v<T> && !std::is_reference_v<T>)
struct MyFuture : std::future<T> {
  struct promise_type : std::promise<T> {
    MyFuture<T> get_return_object() noexcept {
      MyFuture<T> future = {this->get_future()};
      future.promise = this;
      return future;
    }

    Dispatcher initial_suspend() const noexcept { return {}; }
    std::suspend_never final_suspend() const noexcept { return {}; }

    void return_value(const T &value)
    noexcept(std::is_nothrow_copy_constructible_v<T>) {
      this->set_value(value);
      ResumeHandle();
    }
    void return_value(T &&value)
    noexcept(std::is_nothrow_move_constructible_v<T>) {
      this->set_value(std::move(value));
      ResumeHandle();
    }
    void unhandled_exception() noexcept {
      this->set_exception(std::current_exception());
      ResumeHandle();
    }

    MyFuture<T> await_transform(MyFuture<T>(*f)()) {
      return f();
    }

    void CoAwaitOn(std::coroutine_handle<> suspended_handle) {
      std::lock_guard<std::mutex> lock(handle_lock);
      if (is_completed) {
        suspended_handle.resume();
      } else {
        this->handle_ptr = suspended_handle.address();
      }
    }

   private:
    bool is_completed = false;
    std::mutex handle_lock;
    void *handle_ptr = nullptr;

    void ResumeHandle() {
      std::lock_guard<std::mutex> lock(handle_lock);
      is_completed = true;
      if (handle_ptr) {
        std::coroutine_handle<>::from_address(handle_ptr).resume();
        handle_ptr = nullptr;
      }
    }
  };

  promise_type *promise;

};

template<typename T>
auto operator
co_await(
MyFuture<T> future
) noexcept requires (!std::is_reference_v<T>) {
struct Awaiter : MyFuture<T> {
  bool await_ready() const noexcept {
    using namespace std::chrono_literals;
    return this->wait_for(0s) != std::future_status::timeout;
  }

  void await_suspend(std::coroutine_handle<> handle) const {
    this->promise->CoAwaitOn(handle);
  }

  T await_resume() { return this->get(); }
};
return Awaiter{
std::move(future)
};
}

MyFuture<int> compute() {
  int a = co_await []() -> MyFuture<int> {
    this_thread::sleep_for(std::chrono::seconds(1));
    Println(this_thread::get_id(), "a");
    co_return 12;
  };

  Println(this_thread::get_id(), "|a->", a);

  int c = co_await []() -> MyFuture<int> {
    this_thread::sleep_for(std::chrono::seconds(1));
    Println(this_thread::get_id(), "c");
    co_return 10;
  };

  Println(this_thread::get_id(), "|c->", c);

  co_return a * c;
}

MyFuture<int> compute2() {
  Println(this_thread::get_id(), 1);

  auto a = []() -> MyFuture<int> {
    Println(this_thread::get_id(), "a");
    this_thread::sleep_for(std::chrono::seconds(1));
    co_return 12;
  };

  Println(this_thread::get_id(), 2);

  auto c = []() -> MyFuture<int> {
    Println(this_thread::get_id(), "c");
    this_thread::sleep_for(std::chrono::seconds(1));
    co_return 10;
  };

  Println(this_thread::get_id(), 3);;

  co_return (co_await a) * (co_await c);
}

int HardWork(int param) {
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(10000ms);
  return param + 10;
}

int main() {
  Println(this_thread::get_id(), "main");
  auto future = compute();
  Println(this_thread::get_id(), future.get());
  return 0;
}
