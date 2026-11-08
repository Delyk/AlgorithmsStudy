cmake_minimum_required(VERSION 3.20)
project(binary_search CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)

enable_testing()

add_executable(main test.cpp binary_search.cpp)
target_link_libraries(main GTest::gtest)

include(GoogleTest)
gtest_discover_tests(main)