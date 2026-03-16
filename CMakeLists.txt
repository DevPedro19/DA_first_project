cmake_minimum_required(VERSION 3.26)
project(OrganizationalTool)

set(CMAKE_CXX_STANDARD 17)
set( CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )
add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)
set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_TMP} )

add_executable(main
        src/main.cpp
        src/CLI.cpp
        src/Reviewer.cpp
        src/Submission.cpp
        src/CsvParser.cpp
        src/InfoMenu.cpp
        src/OutputWriter.cpp
        src/MaxFlowSolver.cpp
        src/Graph.cpp
)

set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Test
        test/CsvparserTest.cpp
        src/CsvParser.cpp
        src/Submission.cpp
        src/Reviewer.cpp
)

add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)
#set(CMAKE_CXX_FLAGS_DEBUG "/MDd")  # Para Debug
#set(CMAKE_CXX_FLAGS_RELEASE "/MD") # Para Release
target_link_libraries(Test gtest gtest_main gmock gmock_main)
target_include_directories(main PUBLIC ${CMAKE_SOURCE_DIR}/include)
