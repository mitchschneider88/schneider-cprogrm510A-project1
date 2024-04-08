cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)

project(Schneider-Project1 VERSION 1.0)

set(SOURCE_FILES
    src/Main.cpp src/Synthesizer.cpp src/Synthesizer.h src/WavFileManager.cpp src/WavFileManager.h src/WavHeader.h)

add_executable(${PROJECT_NAME}.exe ${SOURCE_FILES})