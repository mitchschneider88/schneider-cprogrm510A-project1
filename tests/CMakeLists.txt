set(BINARY ${CMAKE_PROJECT_NAME}_test)

include_directories(../src)
include_directories(../include/CppUnitLite)

set(TEST_SOURCES
        RunTests.cpp
        Tests.cpp
        CppUnitLite/Failure.cpp
        CppUnitLite/Test.cpp
        CppUnitLite/TestRegistry.cpp
        CppUnitLite/TestResult.cpp
        CppUnitLite/WFailure.cpp)

set(SOURCES ${TEST_SOURCES})

add_executable(${BINARY} ${TEST_SOURCES})

target_link_libraries(${BINARY} PRIVATE synth)

