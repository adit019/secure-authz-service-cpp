#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <optional>

namespace auth {

struct UserRecord {
  std::string username;
  std::string salt;
  std::string password_hash_hex; // SHA-256(salt || password)
  std::unordered_set<std::string> roles;
};

class InMemoryUserStore {
public:
  bool upsert(const UserRecord& rec);
  std::optional<UserRecord> get(const std::string& username) const;
private:
  mutable std::mutex mtx_;
  std::unordered_map<std::string, UserRecord> db_;
};

// cryptographic helpers
std::string generate_salt(size_t len = 16);
std::string sha256_hex(const std::string& data); // uses OpenSSL if available

// high-level API
bool register_user(InMemoryUserStore& store,
                   const std::string& username,
                   const std::string& password,
                   const std::unordered_set<std::string>& roles);

bool authenticate(const InMemoryUserStore& store,
                  const std::string& username,
                  const std::string& password);

bool authorize(const InMemoryUserStore& store,
               const std::string& username,
               const std::string& role);

} // namespace auth
