#ifndef FPFFT_MATH_SINCOS_HPP
#define FPFFT_MATH_SINCOS_HPP

// Sine and cosine functions

#include "clamp.hpp"
#include "fpfft/numbers/fixed_point.hpp" // IWYU pragma: keep
#include <cmath>

namespace FPFFT
{

// Computes the value of sin(2*pi*x).
template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> Sin2PI(const FixedPoint<S, F, I> x)
{
  using FP = FixedPoint<S, F, I>;
  // This function f(x) only approximates over 0 < x < pi/2
  // 3pi/2 < x <= 2pi   : sin(x) = -f(-x+2pi)
  // pi    < x <= 3pi/2 : sin(x) = -f(x-pi)
  // pi/2  < x <= pi    : sin(x) =  f(pi-x)
  // 0     < x <= pi/2  : sin(x) =  f(x)
  // Also sin(-x) = -sin(x)
  const S storage = x.Storage();
  bool negative = storage < 0;
  I X = std::abs(I(storage));
  X %= FP::Scale;
  negative = negative != X > FP::Scale / 2;
  if (3 * FP::Scale / 4 < X)
    X = FP::Scale - X;
  else if (FP::Scale / 2 < X)
    X = X - FP::Scale / 2;
  else if (FP::Scale / 4 < X)
    X = FP::Scale / 2 - X;

  // See cosine function for more detail on derivation
  // sin(x) = Σ ( (-1)^n x^2n+1) / (2n+1)!
  // Where a = 2pi, b = (2pi)^3/6, c = (2pi)^5/120, d = (2pi)^7/5040
  // sin(x)/s = a xs^-1 - b x^3s^-3 + c x^5s^-5 - d x^7s^-7
  // sin(x)/s = a xs^-1 - b x^3s^-3 + x^5s^-5 (c - d x^2s^-2)
  // sin(x)/s = a xs^-1 - x^3s^-3 (b - x^2s^-2 (c - d x^2s^-2))
  // sin(x)/s = xs^-1 (a - x^2s^-2 (b - x^2s^-2 (c - d x^2s^-2)))
  // sin(x) = x (a - x^2s^-2 (b - x^2s^-2 (c - d x^2s^-2)))
  // l = (c - d x^2s^-2) = 2^-e (c 2^e - d 2^e x^2s^-2)
  // m = (b - x^2s^-2 l) = 2^-f (b 2^f -  2^f x^2s^-2 l)
  // n = x (a - x^2s^-2 m) = x 2^-g (a 2^g - 2^g x^2s^-2 m)
  // sin(t) = x 2^-g (
  //   a 2^g - 2^g-2n-f x^2 (
  //     b 2^f - 2^f-2n-e x^2 (
  //       c 2^e - d 2^e-2n x^2)))
  constexpr int n = F;
  // Redefine A = a*2^g, B = b 2^f, C = c 2^e, D = d 2^e
  // sin(t) = x 2^-g (
  //   A - 2^g-2n-f x^2 (
  //     B - 2^f-2n-e x^2 (
  //       C - D 2^-2n x^2)))
  constexpr int e = 10;
  constexpr int f = 10;
  constexpr int g = 10;
  // pow is not a constexpr so the (2pi)^n have to be multiplied out manually
  constexpr I A = (2 * M_PI) * (I(1) << g);
  constexpr I B = ((2 * M_PI) * (2 * M_PI) * (2 * M_PI) / 6) * (I(1) << g);
  constexpr I C =
      ((2 * M_PI) * (2 * M_PI) * (2 * M_PI) * (2 * M_PI) * (2 * M_PI) / 120) * (I(1) << e);
  constexpr I D = ((2 * M_PI) *
                      (2 * M_PI) *
                      (2 * M_PI) *
                      (2 * M_PI) *
                      (2 * M_PI) *
                      (2 * M_PI) *
                      (2 * M_PI) /
                      5040) *
                  (I(1) << e);
  // sin(t) = 2^-g x (
  //   A - 2^g-n-f x 2^-n x (
  //     B - 2^f-n-e x 2^-n x (
  //       C - ^-n x 2^-n D x)))

  // C - ^-n x 2^-n D x
  I y = (X * D) >> n;
  y = (y * X) >> n;
  y = C - y;
  // B - 2^f-n-e x 2^-n x (y)
  y = (y * X) >> n;
  y = (y * X) >> (-f + n + e);
  y = B - y;
  // A - 2^g-n-f x 2^-n x (y)
  y = (y * X) >> n;
  y = (y * X) >> (-g + n + f);
  y = A - y;
  // 2^-g x (y)
  y = (y * X) >> g;

  if (negative)
    y = -y;
  FP out;
  out.Storage() = Clamp<S>(y);
  return out;
}

// Computes the value of cos(2*pi*x).
template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> Cos2PI(const FixedPoint<S, F, I> x)
{
  using FP = FixedPoint<S, F, I>;
  // This function f(x) only approximates over 0 < x < pi/2
  // 3pi/2 < x <= 2pi   : cos(x) =  f(2pi-x)
  // pi    < x <= 3pi/2 : cos(x) = -f(x-pi)
  // pi/2  < x <= pi    : sin(x) = -f(pi-x)
  // 0     < x <= pi/2  : cos(x) =  f(x)
  // Also cos(-x) = cos(x)
  I X = std::abs(I(x.Storage()));
  X %= FP::Scale;
  bool negative = FP::Scale / 4 < X && X < 3 * FP::Scale / 4;
  if (3 * FP::Scale / 4 < X)
    X = FP::Scale - X;
  else if (FP::Scale / 2 < X)
    X = X - FP::Scale / 2;
  else if (FP::Scale / 4 < X)
    X = FP::Scale / 2 - X;

  // cos(x) = Σ ( (-1)^n x^2n) / (2n)!
  // t = (2pi * x)
  // cos(t)/s = Σ ( (-1)^n t^2n s^-2n) / (2n)!             (Factor in scaling)
  // cos(t)/s ~~ 1 - t^2s^-2/2! + t^4s^-4/4! - t^6s^-6/6!  (First 4 terms)
  // cos(t)/s = 1 - ax^2s^-2 + bx^4s^-4 - cx^6s^-6         (Substitute)
  // Where a = (2pi)^2/2, b = (2pi)^4/24, c = (2pi)^6/720
  // cos(t)/s = 1 - x^2s^-2(a - x^2s^-2 (b - cx^2s^-2) )   (Rearrange)
  // Scale to make use of more bits
  // l = (b - cx^2s^-2) = 2^-d (2^d b - 2^d c x^2s^-2)
  // m = (a - x^2s^-2 l) = 2^-e (2^e a - 2^e x^2s^-2 l)
  // cos(t)/s = (
  //   1 - x^2s^-2 2^-e (
  //     2^e a - 2^e x^2s^-2 2^-d (
  //       2^d b - 2^d c x^2s^-2)))
  // Scaling factor is a power of 2 (s = 2^n)
  const int n = F;
  // cos(t) 2^-n = (
  //   1 - 2^-2n-e x^2 (
  //     2^e a - 2^e-2n-d x^2 (
  //       2^d b - 2^d-2n c x^2)))
  // Redefine A = a*2^e, B = b 2^d, C = c 2^d
  // cos(t) = (
  //   2^n - 2^-n-e x^2 (
  //     A - 2^e-2n-d x^2 (
  //       B - C 2^-2n x^2)))
  const int d = 10;
  const int e = 10;
  // pow is not a constexpr so the (2pi)^n have to be multiplied out manually
  const I A = ((2 * M_PI) * (2 * M_PI) / 2) * (I(1) << e);
  const I B = ((2 * M_PI) * (2 * M_PI) * (2 * M_PI) * (2 * M_PI) / 24) * (I(1) << d);
  const I C = ((2 * M_PI) * (2 * M_PI) * (2 * M_PI) * (2 * M_PI) * (2 * M_PI) * (2 * M_PI) / 720) *
              (I(1) << d);
  // Shuffle operations around
  // cos(t) = (
  //   2^n - 2^-e x 2^-n x (
  //     A - 2^e-n-d x 2^-n x (         (Note: A scaled by 2^e)
  //       B - 2^-n x 2^-n C x)))       (Note: B, C scaled by 2^d)
  // B - 2^-n x 2^-n C x
  I y = (X * C) >> n;
  y = (y * X) >> n;
  y = B - y;
  // A - 2^e-n-d x 2^-n x (y)
  y = (y * X) >> n;
  y = (y * X) >> (n + d - e);
  y = A - y;
  // 2^n - 2^-e x 2^-n x (y)
  y = (y * X) >> n;
  y = (y * X) >> e;
  y = (I(1) << n) - y;

  if (negative)
    y = -y;
  FP out;
  out.Storage() = Clamp<S>(y);
  return out;
}

} // namespace FPFFT

#endif
