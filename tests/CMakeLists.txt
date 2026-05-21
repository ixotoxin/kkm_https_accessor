cmake_minimum_required(VERSION 3.28)

find_package(Catch2 CONFIG REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")

file(GLOB_RECURSE TEST_LIBRARY_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/library/lib/*.cpp")

add_subdirectory("lib")
add_subdirectory("ccy")
add_subdirectory("json")
