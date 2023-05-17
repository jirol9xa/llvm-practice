cmake_minimum_required(VERSION 3.14)

set (INCLUDEDIR include/)
set (SRCDIR src/)
set (TESTDIR test/pic)

SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -g -ggdb3 -std=c++17")

project(CallGraph)
add_executable(CallGraph ${SRCDIR}/main.cpp)
add_executable(test ${SRCDIR}/test.cpp)
add_library(parser STATIC ${SRCDIR}/parser.cpp ${SRCDIR}/lib.cpp)
target_include_directories(parser PRIVATE ${INCLUDEDIR})
target_include_directories(test PRIVATE ${INCLUDEDIR})

target_include_directories(CallGraph PRIVATE ${INCLUDEDIR})
target_link_libraries(CallGraph PRIVATE parser)
target_link_libraries(test PRIVATE parser gtest gmock pthread)
