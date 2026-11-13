cmake_minimum_required(VERSION 3.10)
project(heap CXX)

find_package(GTest)

enable_testing()

add_executable(main test_heap.cpp)
target_link_libraries(main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(main)