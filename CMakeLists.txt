cmake_minimum_required(VERSION 3.28)
project(kkm_https_accessor VERSION 1.9.3 LANGUAGES CXX)

set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_FIND_DEBUG_MODE ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "$<1:${PROJECT_BINARY_DIR}/_Archive_${CMAKE_BUILD_TYPE}>" CACHE PATH "Archive")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "$<1:${PROJECT_BINARY_DIR}/_Binary_${CMAKE_BUILD_TYPE}>" CACHE PATH "Binary")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<1:${PROJECT_BINARY_DIR}/_Library_${CMAKE_BUILD_TYPE}>" CACHE PATH "Library")

option(BUILD_SEPARATED "Build separated" OFF)
option(BUILD_STATIC "Build static" OFF)
option(BUILD_TESTS "Build tests" OFF)
option(WITH_SHORTCUTS "Build with command shortcuts enabled" OFF)
option(WITH_RELSL "Enable relative paths for the source location" ON)
option(WITH_SBIAC "Enable invasive access to the std::string buffer" OFF)
option(WITH_CRTD "Build with memory profiling (CRT Debug)" OFF)
option(WITH_SNTZ "Build with sanitizers (AddressSanitizer + UndefinedBehaviorSanitizer)" OFF)
option(WITH_LEAKS "Build with artificial memory leaks" OFF)
option(WITH_10106 "Restrict functionality to driver version 10.10.6" OFF)
option(WITH_10107 "Restrict functionality to driver version 10.10.7" OFF)
option(WITH_10108 "Restrict functionality to driver version 10.10.8" ON)

string(TIMESTAMP KKMHA_BUILD_TIMESTAMP "%Y-%m-%d %H:%M:%S")
set(KKMHA_BUILD_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(KKMHA_DEPS_FPTR10 "${CMAKE_CURRENT_SOURCE_DIR}/deps/fptr10")

set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")

if (BUILD_STATIC)
    message(STATUS "Build static executables")
    set(KKMHA_BUILD_TYPE "Static/${CMAKE_BUILD_TYPE}")
    set(KKMHA_DEPS_VCPKG "${CMAKE_CURRENT_SOURCE_DIR}/deps/static/vcpkg_installed/x64-windows-static")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
else ()
    set(KKMHA_BUILD_TYPE "Dynamic/${CMAKE_BUILD_TYPE}")
    set(KKMHA_DEPS_VCPKG "${CMAKE_CURRENT_SOURCE_DIR}/deps/dynamic/vcpkg_installed/x64-windows")
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    set(KKMHA_DEPS_VCPKG_DLL "${KKMHA_DEPS_VCPKG}/$<$<CONFIG:Debug>:debug/>bin")
endif ()

set(OPENSSL_ROOT_DIR ${KKMHA_DEPS_VCPKG})

find_package(OpenSSL REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")
find_package(asio REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")
find_package(nlohmann_json REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")

set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF /OPT:ICF")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 /GL /Gy /DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od /Ob0 /Oy- /RTC1 /Zi /DDEBUG /D_DEBUG")
    add_compile_options(/W4 /WX /EHsc /Zc:preprocessor /utf-8 /await:strict)
    add_compile_options(/wd4068 /wd4244 /wd4267 /wd4310 /wd4996 /wd4324)
    set(WITH_SNTZ OFF)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} -flto -Wl,/LTCG -Wl,/OPT:REF -Wl,/OPT:ICF")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG")
        set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -DDEBUG -D_DEBUG")
        add_compile_options(
            #-nostdlib -nodefaultlibs
            -Werror -Wall -Wextra -Wpedantic
            -Wcast-align -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wenum-compare -Wfloat-equal
            -Wnon-virtual-dtor -Woverloaded-virtual -Wredundant-decls -Wsign-conversion -Wsign-promo
            -Wno-c2y-extensions -Wno-deprecated-declarations -Wno-implicit-int-conversion
            -Wno-sign-compare -Wno-sign-conversion -Wno-unknown-pragmas
        )
        if (WITH_RELSL)
            add_compile_options("-fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/=.\\")
        endif ()
        if (BUILD_STATIC)
            add_compile_options(-static)
            set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")
        endif ()
    elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS_RELEASE "/O2 /Ob2 -DNDEBUG")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/O2 /Ob2 -g -DNDEBUG")
        set(CMAKE_CXX_FLAGS_DEBUG "/Od -g -DDEBUG -D_DEBUG")
        add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS)
        #add_definitions(-D_SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING)
        add_compile_options(
            #/clang:-nostdlib /clang:-nodefaultlibs
            /clang:-Werror /clang:-Wall /clang:-Wextra /clang:-Wpedantic
            /clang:-Wcast-align /clang:-Wcast-qual /clang:-Wconversion /clang:-Wctor-dtor-privacy /clang:-Wenum-compare
            /clang:-Wfloat-equal /clang:-Wnon-virtual-dtor /clang:-Woverloaded-virtual /clang:-Wredundant-decls
            /clang:-Wsign-conversion /clang:-Wsign-promo /clang:-Wno-c2y-extensions /clang:-Wno-deprecated-declarations
            /clang:-Wno-implicit-int-conversion /clang:-Wno-sign-compare /clang:-Wno-sign-conversion
            /clang:-Wno-unknown-pragmas
        )
        if (WITH_RELSL)
            add_compile_options("/clang:-fmacro-prefix-map=${CMAKE_CURRENT_SOURCE_DIR}/=.\\")
        endif ()
        if (BUILD_STATIC)
        endif ()
    else ()
        message(FATAL_ERROR "Unsupported compiler")
    endif ()
else ()
    message(FATAL_ERROR "Unsupported compiler")
endif ()

add_definitions(/DUNICODE /D_UNICODE /DWIN32_LEAN_AND_MEAN /DNOMINMAX /D_WIN32_WINNT=0x0601)

if (WITH_CRTD)
    if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR "Allowed only in Debug build type")
    endif ()
    set(BUILD_TESTS OFF)
    set(WITH_SNTZ OFF)
    add_definitions(-D_CRTDBG_MAP_ALLOC=1)
endif ()

if (WITH_SNTZ)
    if (NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
        message(FATAL_ERROR "Allowed only in RelWithDebInfo build type")
    endif ()
    if ("$ENV{CLANG_SANLIB_DIR}" STREQUAL "")
        message(FATAL_ERROR "The CLANG_SANLIB_DIR environment variable is required")
    else ()
        message(STATUS "Sanitizers library directory: $ENV{CLANG_SANLIB_DIR}")
    endif()
    string(REPLACE "-nostartfiles" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    string(REPLACE "-nostdlib" "" CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    string(REPLACE "-nostartfiles" "" CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE}")
    string(REPLACE "-nostdlib" "" CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE}")
    link_directories(BEFORE "$ENV{CLANG_SANLIB_DIR}")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
        add_compile_options(
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -fno-sanitize-merge
            -fsanitize-recover=address,undefined
    )
    elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
        set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Oy-")
        add_compile_options(
            -fsanitize=address,undefined
            -fno-sanitize-merge
            -fsanitize-recover=address,undefined
        )
    endif()
    function (target_sntzr_link_options target)
        if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU")
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
            target_link_options(${target} PRIVATE -Wl,/INCREMENTAL:NO)
            #target_link_options(${target} PRIVATE -Wl,/WHOLEARCHIVE:clang_rt.asan_dynamic-x86_64.lib)
            #target_link_options(${target} PRIVATE -Wl,/WHOLEARCHIVE:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)
            #target_link_options(${target} PRIVATE -Wl,/WHOLEARCHIVE:clang_rt.ubsan_standalone-x86_64.lib)
            #target_link_options(${target} PRIVATE -Wl,/WHOLEARCHIVE:clang_rt.ubsan_standalone_cxx-x86_64.lib)
        elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
            target_link_options(${target} PRIVATE /INCREMENTAL:NO)
            target_link_options(${target} PRIVATE /WHOLEARCHIVE:clang_rt.asan_dynamic-x86_64.lib)
            target_link_options(${target} PRIVATE /WHOLEARCHIVE:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)
            #target_link_options(${target} PRIVATE /WHOLEARCHIVE:clang_rt.ubsan_standalone-x86_64.lib)
            #target_link_options(${target} PRIVATE /WHOLEARCHIVE:clang_rt.ubsan_standalone_cxx-x86_64.lib)
        endif()
        if (NOT BUILD_STATIC)
            target_link_libraries(${target} PRIVATE DbgHelp.lib)
            target_link_libraries(${target} PRIVATE Crypt32.lib)
        endif()
        add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
                copy_if_different
                "$ENV{CLANG_SANLIB_DIR}/clang_rt.asan_dynamic-x86_64.dll"
                "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/clang_rt.asan_dynamic-x86_64.dll"
        )
    endfunction ()
    set(BUILD_TESTS OFF)
endif()

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/fptr10")

set(KKMHA_CONFIG_INCLUDE_DIR "${PROJECT_BINARY_DIR}/_Include_${CMAKE_BUILD_TYPE}")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/options.h.in" "${KKMHA_CONFIG_INCLUDE_DIR}/cmake/options.h")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/variables.h.in" "${KKMHA_CONFIG_INCLUDE_DIR}/cmake/variables.h")
include_directories(BEFORE "${KKMHA_CONFIG_INCLUDE_DIR}")

file(GLOB_RECURSE LIBRARY_SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/library/*/*.cpp")
include_directories(BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/src/library")

set(
    MTLIB_PUBLIC_DEFS
        JSON_USE_IMPLICIT_CONVERSIONS=0
        ASIO_STANDALONE
        ASIO_NO_DEPRECATED
        ASIO_NO_WIN32_LEAN_AND_MEAN
        ASIO_NO_NOMINMAX
        ASIO_HAS_THREADS
        ASIO_HAS_STD_COROUTINE
        ASIO_HAS_CO_AWAIT
)

if (BUILD_SEPARATED)
    list(APPEND
        MTLIB_PUBLIC_DEFS
            EXTERNAL_LIB_VARIABLES
            EXTERNAL_MAIN_VARIABLES
            EXTERNAL_LOG_VARIABLES
            EXTERNAL_KKM_VARIABLES
            EXTERNAL_CONF_VARIABLES
    )
else ()
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmop")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmjl")
endif ()

set(STLIB_PUBLIC_DEFS ${MTLIB_PUBLIC_DEFS} SINGLE_THREAD)
set(MTLIB_PRIVATE_DEFS)
set(STLIB_PRIVATE_DEFS)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/library")

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif ()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmha")

if (BUILD_SEPARATED)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmop")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/kkmjl")
endif ()

#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/heap")
