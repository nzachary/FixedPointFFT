#ifndef FPFFT_VERSION_HPP
#define FPFFT_VERSION_HPP

#include "preprocess_utils.hpp"

#define FPFFT_VERSION_MAJOR 0
#define FPFFT_VERSION_MINOR 0
#define FPFFT_VERSION_PATCH 1

#define FPFFT_VERSION_STRING                                                                       \
  _FPFFT_STRINGIFY(FPFFT_VERSION_MAJOR)                                                            \
  "." _FPFFT_STRINGIFY(FPFFT_VERSION_MINOR) "." _FPFFT_STRINGIFY(FPFFT_VERSION_PATCH)

#endif
