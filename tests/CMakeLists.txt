cmake_minimum_required(VERSION 3.28)

find_package(Catch2 CONFIG REQUIRED PATHS "${KKMHA_DEPS_VCPKG}")

file(GLOB_RECURSE TEST_LIBRARY_SOURCE_FILES "${CMAKE_SOURCE_DIR}/src/library/lib/*.cpp")

add_library(utlib OBJECT ${TEST_LIBRARY_SOURCE_FILES})
target_compile_definitions(utlib PUBLIC JSON_USE_IMPLICIT_CONVERSIONS=0)
target_link_libraries(utlib PRIVATE nlohmann_json::nlohmann_json)

add_executable(test_lib_wconv lib_wconv.cpp)
target_link_libraries(test_lib_wconv PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_wconv COMMAND test_lib_wconv)

add_executable(test_lib_defer lib_defer.cpp)
target_link_libraries(test_lib_defer PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_defer COMMAND test_lib_defer)

add_executable(test_lib_numeric lib_numeric.cpp)
target_link_libraries(test_lib_numeric PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_numeric COMMAND test_lib_numeric)

add_executable(test_lib_datetime lib_datetime.cpp)
target_link_libraries(test_lib_datetime PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_datetime PRIVATE utlib)
add_test(NAME test_lib_datetime COMMAND test_lib_datetime)

add_executable(test_lib_text lib_text.cpp)
target_link_libraries(test_lib_text PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_text PRIVATE utlib)
add_test(NAME test_lib_text COMMAND test_lib_text)

add_executable(test_lib_path lib_path.cpp)
target_link_libraries(test_lib_path PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_path COMMAND test_lib_path)

add_executable(test_lib_hexer lib_hexer.cpp)
target_link_libraries(test_lib_hexer PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_hexer COMMAND test_lib_hexer)

add_executable(test_lib_json lib_json.cpp)
target_compile_definitions(test_lib_json PUBLIC JSON_USE_IMPLICIT_CONVERSIONS=0)
target_link_libraries(test_lib_json PRIVATE Catch2::Catch2WithMain)
target_link_libraries(test_lib_json PRIVATE utlib)
add_test(NAME test_lib_json COMMAND test_lib_json)

add_executable(test_ccy_spinlock_p ccy_spinlock_p.cpp)
target_link_libraries(test_ccy_spinlock_p PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_p COMMAND test_ccy_spinlock_p)

add_executable(test_ccy_spinlock_y ccy_spinlock_y.cpp)
target_link_libraries(test_ccy_spinlock_y PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_y COMMAND test_ccy_spinlock_y)

add_executable(test_ccy_spinlock_w ccy_spinlock_w.cpp)
target_link_libraries(test_ccy_spinlock_w PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_w COMMAND test_ccy_spinlock_w)

add_executable(test_ccy_spinlock_a ccy_spinlock_a.cpp)
target_link_libraries(test_ccy_spinlock_a PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_a COMMAND test_ccy_spinlock_a)

add_executable(test_ccy_dynamique ccy_dynamique.cpp)
target_link_libraries(test_ccy_dynamique PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_dynamique COMMAND test_ccy_dynamique)

add_executable(test_ccy_statique ccy_statique.cpp)
target_link_libraries(test_ccy_statique PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_statique COMMAND test_ccy_statique)

add_executable(test_ccy_dynrespool ccy_dynrespool.cpp)
target_link_libraries(test_ccy_dynrespool PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_dynrespool COMMAND test_ccy_dynrespool)
