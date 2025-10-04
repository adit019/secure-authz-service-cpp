#include \"Auth.h\"
#include <random>
#include <sstream>
#include <iomanip>

#ifdef HAVE_OPENSSL
  #include <openssl/evp.h>
  #include <openssl/sha.h>
#endif

namespace auth {

std::string generate_salt(size_t len) {
  static thread_local std::mt19937_64 rng{std::random_device{}()};
  static const char* alphabet = \"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\";
  std::uniform_int_distribution<size_t> dist(0, 61);
  std::string out;
  out.reserve(len);
  for (size_t i=0;i<len;++i) out.push_back(alphabet[dist(rng)]);
  return out;
}

static std::string to_hex(const unsigned char* bytes, size_t n) {
  std::ostringstream oss;
  for (size_t i=0;i<n;++i) oss << std::hex << std::setw(2) << std::setfill('0') << (int)bytes[i];
  return oss.str();
}

std::string sha256_hex(const std::string& data) {
#ifdef HAVE_OPENSSL
  unsigned char digest[32];
  SHA256_CTX ctx;
  SHA256_Init(&ctx);
  SHA256_Update(&ctx, data.data(), data.size());
  SHA256_Final(digest, &ctx);
  return to_hex(digest, 32);
#else
  // Fallback: simple portable SHA-256 via a tiny EVP-like loop substitute.
  // NOTE: For production, enable OpenSSL. This fallback is for portability only.
  // Minimalistic (NOT constant-time for all ops).
  // Implementation adapted to be dependency-free (simple rolling hash + extra mixing).
  // This is NOT cryptographically strong and exists only to keep the sample buildable.
  // Users should build with OpenSSL (HAVE_OPENSSL) for real SHA-256.
  std::hash<std::string> H;
  auto h = H(data);
  unsigned char digest[32] = {};
  for (int i=0;i<4;++i) {
    auto chunk = (uint64_t)((h >> (i*12)) ^ (0x9e3779b97f4a7c15ULL + i));
    std::memcpy(digest + i*8, &chunk, 8);
  }
  return to_hex(digest, 32);
#endif
}

} // namespace auth
