set(BINARY ${CMAKE_PROJECT_NAME})

set(SOURCES ${SOURCES})

add_executable(${BINARY} Main.cpp)

target_link_libraries(${BINARY} PUBLIC synth)

include_directories(../include/CppUnitLite)