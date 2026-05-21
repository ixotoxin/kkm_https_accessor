cmake_minimum_required(VERSION 3.28)

add_executable(test_ccy_spinlock_p spinlock_p.cpp)
target_link_libraries(test_ccy_spinlock_p PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_p COMMAND test_ccy_spinlock_p)

add_executable(test_ccy_spinlock_y spinlock_y.cpp)
target_link_libraries(test_ccy_spinlock_y PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_y COMMAND test_ccy_spinlock_y)

add_executable(test_ccy_spinlock_w spinlock_w.cpp)
target_link_libraries(test_ccy_spinlock_w PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_w COMMAND test_ccy_spinlock_w)

add_executable(test_ccy_spinlock_a spinlock_a.cpp)
target_link_libraries(test_ccy_spinlock_a PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_spinlock_a COMMAND test_ccy_spinlock_a)

add_executable(test_ccy_dynamique dynamique.cpp)
target_link_libraries(test_ccy_dynamique PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_dynamique COMMAND test_ccy_dynamique)

add_executable(test_ccy_statique statique.cpp)
target_link_libraries(test_ccy_statique PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_statique COMMAND test_ccy_statique)

add_executable(test_ccy_dynrespool dynrespool.cpp)
target_link_libraries(test_ccy_dynrespool PRIVATE Catch2::Catch2WithMain)
add_test(NAME test_ccy_dynrespool COMMAND test_ccy_dynrespool)
