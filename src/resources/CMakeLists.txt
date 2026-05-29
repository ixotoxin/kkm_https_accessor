cmake_minimum_required(VERSION 3.28)

include(variables.cmake)

file(MAKE_DIRECTORY "${ELR_GENERATED_DIR}")

if (BUILD_ELRES)
    add_custom_command(
            OUTPUT "${ELR_RC_FILE}" "${ELR_H_FILE}"
            COMMAND mc.exe -u -U -r "${ELR_GENERATED_DIR}" -h "${ELR_GENERATED_DIR}" "${ELR_MC_FILE}"
            DEPENDS "${ELR_MC_FILE}"
            COMMENT "Generating messages.rc and messages.h from messages.mc"
    )
    add_custom_command(
            OUTPUT "${ELR_RES_FILE}"
            COMMAND rc.exe /i "${ELR_GENERATED_DIR}" /fo "${ELR_RES_FILE}" "${ELR_RC_FILE}"
            DEPENDS "${ELR_RC_FILE}"
            COMMENT "Compiling messages.rc and *.bin to messages.res"
    )
else ()
    add_custom_command(
            OUTPUT "${ELR_RES_FILE}"
            COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different
            "${ELR_PRECOMPILED_DIR}"
            "${ELR_GENERATED_DIR}"
            COMMENT "Copying resources files"
    )
endif ()

add_custom_target(elr_files DEPENDS "${ELR_RES_FILE}")
set_source_files_properties("${ELR_RES_FILE}" PROPERTIES GENERATED TRUE EXTERNAL_OBJECT TRUE LANGUAGE RC)
