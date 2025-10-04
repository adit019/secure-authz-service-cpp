#include <cassert>
#include \"Auth.h\"

using namespace auth;

int main() {
  InMemoryUserStore store;
  assert(register_user(store, \"alice\", \"s3cret\", {\"read\"}));
  assert(authenticate(store, \"alice\", \"s3cret\"));
  assert(!authenticate(store, \"alice\", \"wrong\"));       // negative test
  assert(authorize(store, \"alice\", \"read\"));
  assert(!authorize(store, \"alice\", \"admin\"));          // not granted
  return 0;
}
