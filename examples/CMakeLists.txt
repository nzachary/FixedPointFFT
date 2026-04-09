cmake_minimum_required(VERSION 3.30.0)

project(FPFFTExamples CXX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/../include/")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/deps/")
add_compile_options(-g -O2)

add_executable(example_audio_processing "${CMAKE_CURRENT_SOURCE_DIR}/example_audio_processing.cpp")
