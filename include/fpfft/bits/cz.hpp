#ifndef FPFFT_BITS_CLZ_HPP
#define FPFFT_BITS_CLZ_HPP

// Count leading/trailing zeros

#include <type_traits>

namespace FPFFT
{

template <typename T>
int _Clz(T val)
{
  using UnsignedT = std::make_unsigned_t<T>;
#if defined(__GNUC__)
  return __builtin_clzg(static_cast<UnsignedT>(val));
#elif defined(_WIN32)
  #error unimplemented
#else
  #error unimplemented
#endif
}

template <typename T>
int _Ctz(T val)
{
  using UnsignedT = std::make_unsigned_t<T>;
#if defined(__GNUC__)
  return __builtin_ctzg(static_cast<UnsignedT>(val));
#elif defined(_WIN32)
  #error unimplemented
#else
  #error unimplemented
#endif
}

} // namespace FPFFT

#endif
