cmake_minimum_required(VERSION 3.20)
project(arrays_tests CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest CONFIG REQUIRED)

enable_testing()

add_executable(main test_any.cpp)
target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main)

add_executable(main_d test_dynamic.cpp)
target_link_libraries(main_d PRIVATE GTest::gtest GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(main)
gtest_discover_tests(main_d)