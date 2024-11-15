cmake_minimum_required(VERSION 3.15.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets Test)
include_directories(${Qt5_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE CLIENT_SOURCES
    ../../src/client/*.h
    ../../src/client/*.cpp
)

add_executable(
  client_test
  ${CLIENT_SOURCES}
  TestClient.cpp
)
target_link_libraries(
  client_test
  GTest::gtest GTest::gtest_main
  Qt5::Core Qt5::Network Qt5::Widgets Qt5::Test
)

add_test(NAME ClientTest COMMAND client_test)
