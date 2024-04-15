cmake_minimum_required(VERSION 3.27)

set(CMAKE_CXX_STANDARD 20)

project(Schneider-Project1 VERSION 1.0)

include_directories(./include/CppUnitLite)

set(SOURCE_FILES
        include/CppUnitLite/Failure.h
        include/CppUnitLite/Test.h
        include/CppUnitLite/TestHarness.h
        include/CppUnitLite/TestRegistry.h
        include/CppUnitLite/TestResult.h
        include/CppUnitLite/WFailure.h
        CppUnitLite/Failure.cpp
        CppUnitLite/Test.cpp
        CppUnitLite/TestRegistry.cpp
        CppUnitLite/TestResult.cpp
        CppUnitLite/WFailure.cpp
        src/Main.cpp
        src/Synthesizer.cpp src/Synthesizer.h
        src/WavFileManager.cpp
        src/WavFileManager.h
        src/WavHeader.h src/Tests.cpp
        src/Oscillator.cpp
        src/Oscillator.h
        src/SineOscillator.cpp
        src/SineOscillator.h
        src/FileWriter.h
        src/SynthFileWriter.cpp
        src/SynthFileWriter.h
        src/TestFileWriter.cpp
        src/TestFileWriter.h
)

add_executable(${PROJECT_NAME}.exe ${SOURCE_FILES})

