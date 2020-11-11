add_subdirectory(libs/googletest-master)
include_directories(libs/googletest-master/googletest/include)
include_directories(libs/googletest-master/googlemock/include)
target_link_libraries(Rail_simulator gtest gtest_main)