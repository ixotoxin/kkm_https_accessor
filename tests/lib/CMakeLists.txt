cmake_minimum_required(VERSION 3.28)

add_library(liblib OBJECT ${TEST_LIBRARY_SOURCE_FILES})
target_link_libraries(liblib PUBLIC Catch2::Catch2WithMain)

add_executable(test_lib_wconv wconv.cpp)
target_link_libraries(test_lib_wconv PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_wconv COMMAND test_lib_wconv)

add_executable(test_lib_defer defer.cpp)
target_link_libraries(test_lib_defer PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_defer COMMAND test_lib_defer)

add_executable(test_lib_numeric numeric.cpp)
target_link_libraries(test_lib_numeric PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_numeric COMMAND test_lib_numeric)

add_executable(test_lib_datetime datetime.cpp)
target_link_libraries(test_lib_datetime PRIVATE liblib)
add_test(NAME test_lib_datetime COMMAND test_lib_datetime)

add_executable(test_lib_text text.cpp)
target_link_libraries(test_lib_text PRIVATE liblib)
add_test(NAME test_lib_text COMMAND test_lib_text)

add_executable(test_lib_path path.cpp)
target_link_libraries(test_lib_path PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_path COMMAND test_lib_path)

add_executable(test_lib_hexer hexer.cpp)
target_link_libraries(test_lib_hexer PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_lib_hexer COMMAND test_lib_hexer)
