cmake_minimum_required(VERSION 3.26)
project(Organizational_Tool)

set(CMAKE_CXX_STANDARD 14)
set( CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )
add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)

set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_TMP} )

add_executable(Organizational_Tool
        src/Reviewer.cpp
        src/Submission.cpp
        src/CsvParser.cpp
        test/CsvparserTest.cpp
)
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
#set(CMAKE_CXX_FLAGS_DEBUG "/MDd")  # Para Debug
#set(CMAKE_CXX_FLAGS_RELEASE "/MD") # Para Release
target_link_libraries(Organizational_Tool gtest gtest_main gmock gmock_main)
