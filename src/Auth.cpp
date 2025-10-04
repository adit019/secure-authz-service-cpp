#include \"Auth.h\"
#include <cstring>

namespace auth {

bool InMemoryUserStore::upsert(const UserRecord& rec) {
  std::scoped_lock lk(mtx_);
  db_[rec.username] = rec;
  return true;
}

std::optional<UserRecord> InMemoryUserStore::get(const std::string& username) const {
  std::scoped_lock lk(mtx_);
  auto it = db_.find(username);
  if (it == db_.end()) return std::nullopt;
  return it->second;
}

bool register_user(InMemoryUserStore& store,
                   const std::string& username,
                   const std::string& password,
                   const std::unordered_set<std::string>& roles) {
  UserRecord rec;
  rec.username = username;
  rec.salt = generate_salt();
  rec.password_hash_hex = sha256_hex(rec.salt + password);
  rec.roles = roles;
  return store.upsert(rec);
}

bool authenticate(const InMemoryUserStore& store,
                  const std::string& username,
                  const std::string& password) {
  auto rec = store.get(username);
  if (!rec) return false;
  return rec->password_hash_hex == sha256_hex(rec->salt + password);
}

bool authorize(const InMemoryUserStore& store,
               const std::string& username,
               const std::string& role) {
  auto rec = store.get(username);
  if (!rec) return false;
  return rec->roles.count(role) > 0;
}

} // namespace auth
