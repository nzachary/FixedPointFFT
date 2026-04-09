cmake_minimum_required(VERSION 3.30.0)

project(FixedPointFFT CXX)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include/")
add_compile_options(-g -O0)

add_executable(arithmetic_test "${CMAKE_CURRENT_SOURCE_DIR}/src/arithmetic_test.cpp")
add_executable(complex_test "${CMAKE_CURRENT_SOURCE_DIR}/src/complex_test.cpp")
add_executable(math_functions_test "${CMAKE_CURRENT_SOURCE_DIR}/src/math_functions_test.cpp")
add_executable(fft_test "${CMAKE_CURRENT_SOURCE_DIR}/src/fft_test.cpp")
add_executable(ifft_test "${CMAKE_CURRENT_SOURCE_DIR}/src/ifft_test.cpp")

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/run_tests.sh" "${CMAKE_CURRENT_BINARY_DIR}" COPYONLY)
