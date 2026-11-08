cmake_minimum_required(VERSION 3.20)
project(euclid CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest)

enable_testing()

add_executable(main test.cpp euclid.cpp)
target_link_libraries(main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(main)