cmake_minimum_required(VERSION 3.28)

add_executable(kkmjl kkmjl_main.cpp kkmjl_variables.cpp)
add_dependencies(kkmha regmsg)

if (WITH_SNTZ)
    target_sntz_link_options(kkmjl)
endif()

target_link_libraries(kkmjl PRIVATE stnclib)
target_link_libraries(kkmjl PRIVATE RapidJSON rapidjson)

if (POLICY CMP0207)
    cmake_policy(SET CMP0207 NEW)
endif ()

install(
    TARGETS kkmjl
    COMPONENT kkmjl
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DIRECTORIES "$ENV{CLANG_SANLIB_DIR}"
    DESTINATION .
)
