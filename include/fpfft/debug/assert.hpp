#ifndef FPFFT_DEBUG_ASSERT_HPP
#define FPFFT_DEBUG_ASSERT_HPP

#include "fpfft/preprocess_utils.hpp"

#ifndef FPFFT_NO_ASSERT
  #include <stdexcept> // IWYU pragma: export

  #define _FPFFT_ASSERT_1(condition)                                                               \
    if (!(condition))                                                                              \
      throw std::logic_error("Assert failed: " _FPFFT_STRINGIFY(condition));

  #define _FPFFT_ASSERT_2(condition, message)                                                      \
    if (!(condition))                                                                              \
      throw std::logic_error(message);

  #define _FPFFT_ASSERT_3(condition, type, message)                                                \
    if (!(condition))                                                                              \
      throw type(message);

  #define FPFFT_ASSERT(...)                                                                        \
    _FPFFT_EXPAND(_FPFFT_GET_MACRO_ARGS_3(                                                         \
        __VA_ARGS__, _FPFFT_ASSERT_3, _FPFFT_ASSERT_2, _FPFFT_ASSERT_1)(__VA_ARGS__))

#else
  #define FPFFT_ASSERT(...)
#endif

#endif
