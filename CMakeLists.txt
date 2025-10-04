cmake_minimum_required(VERSION 3.16)
project(secure_authz_service_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(USE_OPENSSL "Use OpenSSL for SHA-256" ON)

if (USE_OPENSSL)
  find_package(OpenSSL)
  if (OpenSSL_FOUND)
    message(STATUS "OpenSSL found: \")
    add_compile_definitions(HAVE_OPENSSL=1)
  else()
    message(WARNING "OpenSSL not found; building without it (fallback hasher).")
  endif()
endif()

add_library(auth_core
  src/Auth.cpp
  src/Hash.cpp
)
target_include_directories(auth_core PUBLIC include)
if (OpenSSL_FOUND)
  target_link_libraries(auth_core PUBLIC OpenSSL::Crypto)
endif()

add_executable(auth_service src/main.cpp)
target_link_libraries(auth_service PRIVATE auth_core)

# ---- Tests (simple assert-based runner) ----
enable_testing()
add_executable(unit_tests tests/test_auth.cpp)
target_link_libraries(unit_tests PRIVATE auth_core)
add_test(NAME auth_unit_tests COMMAND unit_tests)

# ---- Load test (500+ concurrent sessions) ----
add_executable(load_test tools/load_test.cpp)
target_link_libraries(load_test PRIVATE auth_core)
