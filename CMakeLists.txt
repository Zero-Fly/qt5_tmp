cmake_minimum_required(VERSION 3.15.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "qt-multithreading")
set(PROJECT_EXE ${PROJECT_NAME}_exe)
project(${PROJECT_NAME} VERSION 1.0.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)

file(GLOB_RECURSE PROJECT_SOURCES
    src/*.h
    src/*.cpp
)

add_executable(${PROJECT_EXE} ${PROJECT_SOURCES})

enable_testing()
add_subdirectory(test/server)
add_subdirectory(test/client)

target_link_libraries(${PROJECT_EXE} Qt5::Core Qt5::Network Qt5::Widgets)
