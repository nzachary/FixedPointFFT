#ifndef FPFFT_NUMBERS_SFINAE_HPP
#define FPFFT_NUMBERS_SFINAE_HPP

#include "forward_decl.hpp"

namespace FPFFT
{

template <typename T>
struct IsFixed
{
  static constexpr bool value = false;
};

template <typename S, int F, typename I>
struct IsFixed<FixedPoint<S, F, I>>
{
  static constexpr bool value = true;
};

} // namespace FPFFT

#endif
