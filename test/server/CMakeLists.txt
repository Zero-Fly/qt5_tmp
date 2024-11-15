cmake_minimum_required(VERSION 3.15.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets Test)
include_directories(${Qt5_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SERVER_SOURCES
    ../../src/server/*.h
    ../../src/server/*.cpp
)

add_executable(
  server_test
  ${SERVER_SOURCES}
  TestServer.cpp

)
target_link_libraries(
  server_test
  GTest::gtest GTest::gtest_main
  Qt5::Core Qt5::Network Qt5::Widgets Qt5::Test
)

add_test(NAME ServerTest COMMAND server_test)
