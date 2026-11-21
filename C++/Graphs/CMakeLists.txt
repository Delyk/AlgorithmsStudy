cmake_minimum_required(VERSION 3.10)
project(graph CXX)

find_package(GTest)

enable_testing()

add_executable(main test.cpp)
target_link_libraries(main GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(main)