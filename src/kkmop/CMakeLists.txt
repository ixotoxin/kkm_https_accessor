cmake_minimum_required(VERSION 3.28)

add_executable(kkmop kkmop_main.cpp kkmop_variables.cpp)
add_dependencies(kkmha regmsg)

if (WITH_SNTZ)
    target_sntzr_link_options(kkmop)
endif()

target_link_libraries(kkmop PRIVATE stlib)
target_link_libraries(kkmop PRIVATE nlohmann_json::nlohmann_json)

install(
    TARGETS kkmop
    COMPONENT kkmop
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DIRECTORIES "$ENV{CLANG_SANLIB_DIR}"
    DESTINATION .
)

if (BUILD_TESTS)
    add_test(NAME test_kkmop COMMAND kkmop help)
endif ()
