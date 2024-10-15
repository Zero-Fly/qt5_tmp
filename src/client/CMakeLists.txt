cmake_minimum_required(VERSION 3.15.0)
set(CLIENT_SOURCES tClient.cpp)
set(CLIENT_HEADERS tClient.h)
# Specify the UI files
set(FORMS
    tClient.ui
)

# Generate the UI header files
qt5_wrap_ui(UI_HEADERS ${FORMS})

add_library(tClient ${CLIENT_SOURCES} ${CLIENT_HEADERS} ${UI_HEADERS})
target_include_directories(tClient PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
