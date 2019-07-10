cmake_minimum_required(VERSION 3.12)
SET(PROJECT_NAME "05_sparseMatrix")
project(${PROJECT_NAME})

include_directories("src")
include_directories("../include")

add_library(gtest src/gtest-all.cc)
add_library(gtest_main src/gtest_main.cc)
target_link_libraries(gtest_main PUBLIC gtest)

add_executable(${PROJECT_NAME} test/gtest.cpp)
target_link_libraries(${PROJECT_NAME} gtest gtest_main)

