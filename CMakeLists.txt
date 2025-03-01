cmake_minimum_required(VERSION 3.13)

project(cast)

file(GLOB cast_sources *.c)

add_executable(test_cast ${cast_sources})

target_compile_options(test_cast PRIVATE -O3)
target_compile_options(test_cast PRIVATE -Wall)
target_compile_options(test_cast PRIVATE -Werror)
target_compile_options(test_cast PRIVATE -Wextra)
target_compile_options(test_cast PRIVATE -Wconversion)
target_compile_options(test_cast PRIVATE -Wfloat-conversion)
target_compile_options(test_cast PRIVATE -Wsign-conversion)
target_compile_options(test_cast PRIVATE -pedantic)

target_compile_features(test_cast PRIVATE c_std_11)
