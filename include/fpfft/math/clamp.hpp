#ifndef FPFFT_MATH_CLAMP_HPP
#define FPFFT_MATH_CLAMP_HPP

// Clamp numbers to a limit

#include "../numbers/numbers.hpp" // IWYU pragma: keep
#include <limits>

namespace FPFFT
{

template <typename Narrow, typename Wide>
constexpr Narrow Clamp(const Wide x)
{
  if (x > std::numeric_limits<Narrow>::max())
    return std::numeric_limits<Narrow>::max();
  else if (x < std::numeric_limits<Narrow>::min())
    return std::numeric_limits<Narrow>::min();
  else
    return x;
}

} // namespace FPFFT

#endif
