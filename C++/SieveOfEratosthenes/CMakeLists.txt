cmake_minimum_required(VERSION 3.20)
project(sieve CXX)

find_package(GTest)

enable_testing()

add_executable(main_t sieve.cpp test.cpp)
target_link_libraries(main_t GTest::gtest)

include(GoogleTest)
gtest_discover_tests(main_t)