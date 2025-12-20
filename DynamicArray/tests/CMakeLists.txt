cmake_minimum_required(VERSION 3.10)
project(ArrayTests)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(ArrayTests
    ArrayTests.cpp
    ../src/Array.ipp
)

target_link_libraries(ArrayTests
    GTest::gtest
    GTest::gtest_main
)

enable_testing()
add_test(NAME ArrayTests COMMAND ArrayTests)

