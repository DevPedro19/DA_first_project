cmake_minimum_required(VERSION 3.26)
project(OrganizationalTool)

set(CMAKE_CXX_STANDARD 17)

# =========================================================================
# 1. CROSS-PLATFORM STATIC LINKING SETTINGS
# Must be placed BEFORE add_subdirectory and add_executable
# =========================================================================
if(MSVC)
    # Visual Studio: Tell CMake to use the Multi-Threaded Static Runtime (/MT instead of /MD)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

    # Prevent GoogleTest from forcing the dynamic runtime (/MD) in MSVC
    set(gtest_force_shared_crt OFF CACHE BOOL "" FORCE)
elseif(WIN32 AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # MinGW on Windows: Statically link C++ standard and system libraries
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
elseif(APPLE)
    # macOS: Do nothing (Apple requires dynamic links to libSystem.dylib)
elseif(UNIX)
    # Linux: Only statically link the C++ standard library (avoids glibc networking crashes)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# =========================================================================
# 2. SUBDIRECTORIES & LIBRARIES
# =========================================================================
set( CMAKE_BUILD_TYPE_TMP "${CMAKE_BUILD_TYPE}" )
set( CMAKE_BUILD_TYPE "Release" )
add_subdirectory(lib/googletest)
include_directories(lib/googletest/googletest/include)
include_directories(lib/googletest/googlemock/include)
set( CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_TMP} )

# =========================================================================
# 3. EXECUTABLES
# =========================================================================
add_library(core STATIC
        src/CLI.cpp
        src/Reviewer.cpp
        src/Submission.cpp
        src/CsvParser.cpp
        src/InfoMenu.cpp
        src/OutputWriter.cpp
        src/MaxFlowSolver.cpp
)
add_executable(main src/main.cpp)

# Output main.exe to the project root
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Use a descriptive target name to avoid PowerShell/CMake conflicts
add_executable(OrgToolTests test/CsvparserTest.cpp
        test/runTests.cpp)

# Output OrgToolTests.exe to the project root
set_target_properties(OrgToolTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# =========================================================================
# 4. TARGET PROPERTIES & LINKING
# =========================================================================
add_definitions(-D_ITERATOR_DEBUG_LEVEL=0)

# Link both libraries and headers to the new OrgToolTests target
target_link_libraries(main PRIVATE core)
target_link_libraries(OrgToolTests PRIVATE gtest gtest_main gmock gmock_main core)

# Give both executables access to your include directory
target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)
