cmake_minimum_required(VERSION 3.15.0)
set(SERVER_SOURCES tServer.cpp)
set(SERVER_HEADERS tServer.h)

add_library(tServer ${CLIENT_SOURCES} ${CLIENT_HEADERS})
target_include_directories(tServer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
