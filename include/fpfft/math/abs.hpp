#ifndef FPFFT_MATH_ABS_HPP
#define FPFFT_MATH_ABS_HPP

// Absolute value function

#include "../numbers/numbers.hpp" // IWYU pragma: keep
#include <limits>

namespace FPFFT
{

template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> Abs(const FixedPoint<S, F, I> x)
{
  S storage = x.Storage();
  if (storage > 0)
    return x;
  // Clamp so it doesn't overflow
  // abs(S_MIN) = S_MAX + 1 and would overflow back to S_MIN
  else if (storage == std::numeric_limits<S>::min())
    storage = std::numeric_limits<S>::max();
  else
    storage = -storage;

  FixedPoint<S, F, I> out;
  out.Storage() = storage;
  return out;
}

} // namespace FPFFT

#endif
