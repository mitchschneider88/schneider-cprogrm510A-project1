set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES ${SOURCES})

add_executable(${BINARY} Main.cpp)

target_link_libraries(${BINARY} PRIVATE synth)

include_directories(../include/CppUnitLite)