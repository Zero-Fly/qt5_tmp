cmake_minimum_required(VERSION 3.15.0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "qt-multithreading")

project(${PROJECT_NAME} VERSION 1.0.0)

find_package(Qt5 REQUIRED COMPONENTS Core Network Widgets)
include_directories(${Qt5_INCLUDE_DIRS})
set(CMAKE_AUTOMOC ON)

QT5_WRAP_CPP(MOC_FILES
    src/client/tclient.cpp
    src/server/tserver.cpp)

#add_subdirectory(src/client)
#add_subdirectory(src/server)



add_executable(main main.cpp ${MOC_Files})

enable_testing()
#add_subdirectory(test/server)
#add_subdirectory(test/client)

target_link_libraries(main Qt5::Core Qt5::Network Qt5::Widgets)
