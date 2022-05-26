include_directories(${CMAKE_HOME_DIRECTORY}/src)
message(STATUS "enable testing.")

add_executable(test_label test_label.cpp)
message(STATUS "\ttesting label")
add_test(NAME testing_label COMMAND test_label)

add_executable(test_config test_config.cpp)
target_link_libraries(test_config ${MAIN_LIB})
message(STATUS "\ttesting config")
add_test(NAME testing_config COMMAND test_config)

add_executable(test_timestamp test_timestamp.cpp)
target_link_libraries(test_timestamp ${MAIN_LIB})
message(STATUS "\ttesting timestamp")
add_test(NAME testing_timestamp COMMAND test_timestamp "2011-09-26 13:02:37.004854985" 1317042157.004854985)
