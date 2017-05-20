#include <experimental/coroutine>
#include <iostream>

namespace exp = std::experimental;
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
#define LOGGED_DESTRUCTOR(C) \
  ~C() { logged_scope s("Destroy " #C); }

struct test {
  LOGGED_DEFAULT_CONSTRUCTOR(test)
  LOGGED_DESTRUCTOR(test)

  struct promise_type {
    LOGGED_DEFAULT_CONSTRUCTOR(promise_type)
    LOGGED_DESTRUCTOR(promise_type)

    struct return_object {
      LOGGED_DEFAULT_CONSTRUCTOR(return_object)
      LOGGED_DESTRUCTOR(return_object)
      operator test() const {
        logged_scope s("Convert return object to return value");
        return test{};
      }
    };

    auto get_return_object() {
      logged_scope s("get_return_object");
      return return_object{};
    }
    auto initial_suspend() {
      logged_scope s("initial_suspend");
      return exp::suspend_never{};
    }
    auto yield_value(int) {
      logged_scope s("yield_value");
      return exp::suspend_never{};
    }
    auto return_void() { logged_scope s("return_void"); }
    auto final_suspend() {
      logged_scope s("final_suspend");
      return exp::suspend_never{};
    }
  };
};

test do_test() {
  co_yield 1;
  co_return;
}

int main(int argc, char* argv[]) { do_test(); }
