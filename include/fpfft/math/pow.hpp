#ifndef FPFFT_MATH_POW_HPP
#define FPFFT_MATH_POW_HPP

#include "../numbers/numbers.hpp" // IWYU pragma: keep
#include "factorial.hpp"

namespace FPFFT
{

// Bring a number up to an integer power
template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> PowI(const FixedPoint<S, F, I> base, const int power)
{
  using FP = FixedPoint<S, F, I>;
  FP x = base;
  int n = power;
  if (power == 0)
  {
    return FP(1);
  }
  else if (power < 0)
  {
    x = FP(1) / base;
    n = -power;
  }

  // TODO: Use something more efficient like the sliding-window method
  FP out(1);
  for (int n = power; n > 1; n /= 2)
  {
    if (n % 2 == 1)
      out *= x;
    x *= x;
  }
  return x * out;
}

template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> Pow(const FixedPoint<S, F, I> base, const FixedPoint<S, F, I> power)
{
  using FP = FixedPoint<S, F, I>;
}

// Natural exponent
template <typename S, int F, typename I, size_t N = 8>
constexpr FixedPoint<S, F, I> Exp(const FixedPoint<S, F, I> power)
{
  // e^x = Σ x^n / n!
  using FP = FixedPoint<S, F, I>;
  FP out(1);
  for (size_t n = 1; n < N; n++)
    out += PowI(power, n) / Factorial(n);
  return out;
}

} // namespace FPFFT

#endif
