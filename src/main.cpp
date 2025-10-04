#include <iostream>
#include \"Auth.h\"

using namespace auth;

int main() {
  InMemoryUserStore store;

  // Bootstrap: create an admin user
  register_user(store, \"admin\", \"admin_pass\", {\"admin\", \"read\", \"write\"});

  std::cout << \"Secure Authentication & Authorization Service (demo CLI)\\n\";
  std::cout << \"Type: register <user> <pass> | login <user> <pass> | check <user> <role> | exit\\n\";

  std::string cmd,u,p,r;
  while (true) {
    std::cout << \"> \";
    if (!(std::cin >> cmd)) break;
    if (cmd == \"exit\") break;
    if (cmd == \"register\" && std::cin >> u >> p) {
      if (register_user(store, u, p, {\"read\"})) std::cout << \"[OK] registered\\n\";
      else std::cout << \"[ERR] register failed\\n\";
    } else if (cmd == \"login\" && std::cin >> u >> p) {
      std::cout << (authenticate(store, u, p) ? \"[OK] auth success\\n\" : \"[ERR] auth failed\\n\");
    } else if (cmd == \"check\" && std::cin >> u >> r) {
      std::cout << (authorize(store, u, r) ? \"[OK] authorized\\n\" : \"[ERR] forbidden\\n\");
    } else {
      std::cout << \"[?] unknown/invalid command\\n\";
      std::cin.clear(); std::cin.ignore(1024, '\\n');
    }
  }
  return 0;
}
