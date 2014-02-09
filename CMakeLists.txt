# Must set version 2.8.1 or later to use relative path for library link
cmake_minimum_required(VERSION 2.8.1)
project(cmakesample C)
set(serial "1.0.0")

enable_testing()

add_subdirectory(krmalloc)
add_subdirectory(krmalloc_test)
add_test(test01 krmalloc_test/krmalloc_test)

