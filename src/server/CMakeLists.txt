cmake_minimum_required(VERSION 3.15.0)

find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)
include_directories(${Qt5_INCLUDE_DIRS})
set(CMAKE_AUTOMOC ON)

add_executable(server tserver.cpp)

target_link_libraries(server Qt5::Core Qt5::Network Qt5::Widgets)
