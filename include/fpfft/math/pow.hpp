#ifndef FPFFT_MATH_POW_HPP
#define FPFFT_MATH_POW_HPP

#include "factorial.hpp"
#include "fpfft/math/sincos.hpp"
#include "fpfft/numbers/fixed_point.hpp" // IWYU pragma: keep

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

// Natural exponent: e^x
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

// Natural exponent of a complex number: e^i(2pi x)
template <typename S, int F, typename I>
constexpr Complex<FixedPoint<S, F, I>> Exp2PIC(const FixedPoint<S, F, I> power)
{
  using FP = FixedPoint<S, F, I>;
  FP real = Cos2PI(power);
  FP imaginary = Sin2PI(power);
  return Complex<FP>(real, imaginary);
}


} // namespace FPFFT

#endif
