include_directories("../json")
include_directories("src")
include_directories("../include")

add_library(gtest src/gtest-all.cc)
add_library(gtest_main src/gtest_main.cc)
target_link_libraries(gtest_main PUBLIC gtest)

add_executable(TestG test/gtest.cpp)
target_link_libraries(TestG gtest gtest_main)

