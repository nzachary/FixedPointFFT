#!/bin/bash

# This script will be automatically copied to the build directory
# Run the script in the build directory instead of the one in the {ROOT}/scripts directory

./arithmetic_test &&
./complex_test &&
./math_functions_test &&
./fft_test &&
./ifft_test
