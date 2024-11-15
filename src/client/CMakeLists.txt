cmake_minimum_required(VERSION 3.15.0)
find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)
include_directories(${Qt5_INCLUDE_DIRS})
set(CMAKE_AUTOMOC ON)
#add_executable(client tclient.cpp)
add_library(client tclient.cpp)

target_link_libraries(client Qt5::Core Qt5::Network Qt5::Widgets)
