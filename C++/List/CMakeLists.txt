cmake_minimum_required(VERSION 3.20)
project(list CXX)

find_package(GTest)

enable_testing()

add_executable(main_single test_single.cpp)
add_executable(main_double test_double.cpp)
target_link_libraries(main_single GTest::gtest GTest::gtest_main)
target_link_libraries(main_double GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(main_single)
gtest_discover_tests(main_double)