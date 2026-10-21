cmake_minimum_required(VERSION 3.24)
project(RetroSurroundPlayer VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

add_executable(RetroSurroundPlayer
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/SurroundProcessor.h
    src/SurroundProcessor.cpp
)
target_link_libraries(RetroSurroundPlayer PRIVATE Qt6::Widgets Qt6::Multimedia)

# Prototype note:
# Qt Multimedia handles playback. The DSP class is an initial matrix-surround
# experiment, not a licensed or certified Dolby Pro Logic II decoder.
