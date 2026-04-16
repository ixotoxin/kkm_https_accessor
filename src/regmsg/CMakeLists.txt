cmake_minimum_required(VERSION 3.28)

add_executable(
    regmsg
        regmsg_main.cpp
        registry.cpp
        "${CMAKE_SOURCE_DIR}/src/library/lib/errexp.cpp"
        "${CMAKE_SOURCE_DIR}/src/library/debug/memprof.cpp"
        "${MSG_RES_FILE}"
)

add_dependencies(regmsg elr_files)

if (WITH_CRTD OR WITH_SNTZ)
    target_compile_definitions(regmsg PRIVATE -DWITHOUT_LOGGING)
    #target_sources(regmsg PRIVATE "${CMAKE_SOURCE_DIR}/src/library/debug/memprof.cpp")
endif()

if (WITH_SNTZ)
    target_sntzr_link_options(regmsg)
endif()

install(
    TARGETS regmsg
    COMPONENT regmsg
    RUNTIME_DEPENDENCIES
    PRE_EXCLUDE_REGEXES "api-ms-" "ext-ms-"
    POST_EXCLUDE_REGEXES ".*system32/.*\\.dll"
    DIRECTORIES "$ENV{CLANG_SANLIB_DIR}"
    DESTINATION .
)
