cmake_minimum_required(VERSION 3.28)

set(ELR_GENERATED_DIR "${GENERATED_DIR}/elr")
set(MSG_MC_FILE "${PROJECT_SOURCE_DIR}/src/regmsg/messages.mc")
set(MSG_RC_FILE "${ELR_GENERATED_DIR}/messages.rc")
set(MSG_RES_FILE "${ELR_GENERATED_DIR}/messages.res")
set(MSG_H_FILE "${ELR_GENERATED_DIR}/messages.h")

file(MAKE_DIRECTORY "${ELR_GENERATED_DIR}")

if (BUILD_ELOGMSG)
    add_custom_command(
        OUTPUT "${MSG_RC_FILE}" "${MSG_H_FILE}"
        COMMAND mc.exe -u -U -r "${ELR_GENERATED_DIR}" -h "${ELR_GENERATED_DIR}" "${MSG_MC_FILE}"
        DEPENDS "${MSG_MC_FILE}"
        COMMENT "Generating messages.rc and messages.h from messages.mc"
    )
    add_custom_command(
        OUTPUT "${MSG_RES_FILE}"
        COMMAND rc.exe /i "${ELR_GENERATED_DIR}" /fo "${MSG_RES_FILE}" "${MSG_RC_FILE}"
        DEPENDS "${MSG_RC_FILE}"
        COMMENT "Compiling messages.rc and *.bin to messages.res"
    )
else ()
    add_custom_command(
        OUTPUT "${MSG_RES_FILE}"
        #TARGET regmsg
        #PRE_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different
            "${PROJECT_SOURCE_DIR}/src/regmsg/resources"
            "${ELR_GENERATED_DIR}"
        COMMENT "Copying resources files"
    )
endif ()

add_custom_target(elr_files DEPENDS "${MSG_RES_FILE}")
set_source_files_properties("${MSG_RES_FILE}" PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE LANGUAGE RC)

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
