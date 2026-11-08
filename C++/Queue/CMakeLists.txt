cmake_minimum_required(VERSION 3.20)
project(queque CXX)

find_package(GTest)

enable_testing()

add_executable(main test.cpp)
target_link_libraries(main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(main)