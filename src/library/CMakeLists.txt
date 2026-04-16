cmake_minimum_required(VERSION 3.28)

set(ELR_GENERATED_DIR "${GENERATED_DIR}/elr")
set(MSG_MC_FILE "${PROJECT_SOURCE_DIR}/src/library/elr/messages.mc")
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
            COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different
            "${PROJECT_SOURCE_DIR}/src/library/elr/resources"
            "${ELR_GENERATED_DIR}"
            COMMENT "Copying resources files"
    )
endif ()

add_custom_target(elr_files DEPENDS "${MSG_RES_FILE}")
set_source_files_properties("${MSG_RES_FILE}" PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE LANGUAGE RC)

add_library(mtlib OBJECT ${LIBRARY_SOURCE_FILES})
add_dependencies(mtlib elr_files)
if (BUILD_SEPARATED)
    target_sources(mtlib PRIVATE variables.cpp)
endif ()
target_compile_definitions(mtlib PUBLIC ${MTLIB_PUBLIC_DEFS} PRIVATE ${MTLIB_PRIVATE_DEFS})
target_link_libraries(mtlib PRIVATE nlohmann_json::nlohmann_json)

add_library(stlib OBJECT ${LIBRARY_SOURCE_FILES})
add_dependencies(stlib elr_files)
if (BUILD_SEPARATED)
    target_sources(stlib PRIVATE variables.cpp)
endif ()
target_compile_definitions(stlib PUBLIC ${STLIB_PUBLIC_DEFS} PRIVATE ${STLIB_PRIVATE_DEFS})
target_link_libraries(stlib PRIVATE nlohmann_json::nlohmann_json)
