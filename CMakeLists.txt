cmake_minimum_required(VERSION 3.27)

project(Schneider-Project1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_library(synth src/Oscillator.cpp src/SineOscillator.cpp src/Synthesizer.cpp src/WavFileManager.cpp)

add_subdirectory(src)
add_subdirectory(tests)
