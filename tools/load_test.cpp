#include <thread>
#include <vector>
#include <atomic>
#include <iostream>
#include \"Auth.h\"

using namespace auth;

int main() {
  InMemoryUserStore store;
  const int users = 600;
  for (int i=0;i<users;++i) {
    register_user(store, \"user\"+std::to_string(i), \"pw\"+std::to_string(i), {\"read\"});
  }
  std::atomic<int> ok{0}, fail{0};
  std::vector<std::thread> ts;
  ts.reserve(users);
  for (int i=0;i<users;++i) {
    ts.emplace_back([&,i]{
      if (authenticate(store, \"user\"+std::to_string(i), \"pw\"+std::to_string(i))) ok++;
      else fail++;
    });
  }
  for (auto& t: ts) t.join();
  std::cout << \"OK=\" << ok.load() << \" FAIL=\" << fail.load() << \"\\n\";
  return fail.load() == 0 ? 0 : 1;
}
