#include <experimental/coroutine>
#include <iostream>
#include <string>

using namespace std::experimental;
using namespace std;

thread_local int t_level;
struct[[nodiscard]] logged_scope {
  logged_scope(string name) : m_name(name) {
    cout << indent() << "-> " << m_name << "\n";
    ++t_level;
  }
  ~logged_scope() {
    --t_level;
    // cout << indent() << "<- " << m_name << "\n";
  }

  string m_name;

  string indent() const { return string(t_level * 2, ' '); }
};

#define LOGGED_DEFAULT_CONSTRUCTOR(C) \
  C() { logged_scope s("Create " #C); }
#define LOGGED_COPY_CONSTRUCTOR(C) \
  C(C const&) { logged_scope s("Copy " #C); }
#define LOGGED_DESTRUCTOR(C) \
  ~C() { logged_scope s("Destroy " #C); }

struct test {
  struct promise_type;

  LOGGED_DEFAULT_CONSTRUCTOR(test)
  test(coroutine_handle<promise_type> h) : h{h} { logged_scope s("Create test"); }
  LOGGED_COPY_CONSTRUCTOR(test)
  LOGGED_DESTRUCTOR(test)

  struct promise_type {
    LOGGED_DEFAULT_CONSTRUCTOR(promise_type)
    LOGGED_DESTRUCTOR(promise_type)

    struct return_object {
      coroutine_handle<promise_type> h;
      LOGGED_DEFAULT_CONSTRUCTOR(return_object)
      return_object(coroutine_handle<promise_type> h) : h(h) {
        logged_scope s("Create return_object");
      }
      LOGGED_DESTRUCTOR(return_object)
      operator test() const {
        logged_scope s("Convert return object to return value");
        return test{h};
      }
    };

    auto get_return_object() {
      logged_scope s("get_return_object");
      return return_object{coroutine_handle<promise_type>::from_promise(*this)};
    }
    auto initial_suspend() {
      logged_scope s("initial_suspend");
      return suspend_never{};
    }
    auto yield_value(int i) {
      logged_scope s("yield_value " + std::to_string(i));
      return suspend_always{};
    }
    auto return_void() { logged_scope s("return_void"); }
    auto final_suspend() {
      logged_scope s("final_suspend");
      return suspend_never{};
    }
  };

  auto resume() {
    logged_scope s("resume");
    h.resume();
  }
  coroutine_handle<promise_type> h;
};

test do_test() {
  logged_scope s("coroutine body");
  co_yield 1;
  co_yield 2;
  co_return;
}

int main(int argc, char* argv[]) {
  auto t = do_test();
  t.resume();
  t.resume();
}
