#ifndef FPFFT_MATH_SQRT_HPP
#define FPFFT_MATH_SQRT_HPP

#include "../bits/bits.hpp"
#include "../containers/lut.hpp"
#include "../numbers/numbers.hpp" // IWYU pragma: keep
#include <climits>
#include <stdexcept>

namespace FPFFT
{

// Estimate the value of a square root
template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> _EstimateSqrt(const FixedPoint<S, F, I> val)
{
  // The estimate can be approximated by (0.485 + 0.485x) * scale
  // Or, even more roughly, (0.485 + 0.5x) * scale
  // This is equal to (0.485 * scale) + (val.Storage() >> 1), since val.Storage() is already scaled
  constexpr S v = 0.485 * FixedPoint<S, F, I>::Scale;
  FixedPoint<S, F, I> out;
  out.Storage() = v + (val.Storage() >> 1);
  return out;
}

// Heron's method https://en.wikipedia.org/wiki/Square_root_algorithms#Heron%27s_method
// X_n+1 = 0.5(x_n + (S / x_n))
// Accuracy improves as n -> infinity
template <size_t N = 8>
struct HeronsSqrt
{
  template <typename S, int F, typename I>
  constexpr static FixedPoint<S, F, I> Sqrt(const FixedPoint<S, F, I> val)
  {
    FixedPoint<S, F, I> x = _EstimateSqrt(val);
    for (size_t n = 0; n < N; n++)
    {
      x = x + val / x;
      x.Storage() >>= 1;
    }
    return x;
  }
};

// Bakhshali method https://en.wikipedia.org/wiki/Square_root_algorithms#Bakhshali_method
// a_n = (S - x_n^2) / 2x_n
// x_n+1 = x_n + a_n
// x_n+2 = x_n+1 - (a_n^2 / 2x_n+1)
template <size_t N = 4>
struct BakhshaliSqrt
{
  template <typename S, int F, typename I>
  constexpr static FixedPoint<S, F, I> Sqrt(const FixedPoint<S, F, I> val)
  {
    FixedPoint<S, F, I> x = _EstimateSqrt(val);
    FixedPoint<S, F, I> a;
    for (size_t n = 0; n < N; n++)
    {
      a = (val - x * x) / (x * 2);
      x = x + a;
      x = x - (a * a) / (x * 2);
    }
    return x;
  }
};

// Use the identity that sqrt(x) = e^0.5ln(x)
struct ExponentialSqrt
{
  template <typename S, int F, typename I>
  constexpr static FixedPoint<S, F, I> Sqrt(const FixedPoint<S, F, I> val)
  {
    FixedPoint<S, F, I> lnx = Ln(val);
    lnx.Storage() >>= 1;
    return Exp(lnx);
  }
};

// Find the square root using a lookup table
template <int VBits = 8>
struct LUTSqrt
{
  template <typename S, int F, typename I>
  constexpr static FixedPoint<S, F, I> Sqrt(const FixedPoint<S, F, I> val)
  {
    using FP = FixedPoint<S, F, I>;
    // Maximum number of leading zeros
    constexpr int SBits = sizeof(S) * CHAR_BIT - VBits;
    constexpr size_t VMax = 1 << VBits;

    static_assert(SBits > 0);
    static_assert(VBits > 0);

    // It is absolutely ridiculous to store up to 2^63 * 8 bytes (for a int64_t based number).
    // Instead, only use the upper part as an index and the number of leading zeros as a scaling
    // factor. That way, only a much smaller subset needs to be stored for this LUT. This _should_
    // still be fairly accurate because it should give more resolution in areas with more leading
    // zeros which is where sqrt(x + 1) - sqrt(x) is larger
    class SqrtLut : public LUT<FP, SBits, VMax>
    {
     public:

      constexpr SqrtLut()
      {
        FP val(0);
        // Signed numbers but should never be negative; MSB should never be set
        for (int lz = 1; lz < SBits; lz++)
        {
          for (int i = 0; i < VMax; i++)
          {
            val.Storage() = i << (SBits - lz);
            this->data(lz, i) = HeronsSqrt<>::Sqrt(val);
          }
        }
      }
    };

    constexpr SqrtLut lut;
    int leadingZeros = _Clz(val.Storage());
    leadingZeros = std::min(leadingZeros, SBits - 1);
    S value = val.Storage() >> (SBits - leadingZeros);
    return lut(leadingZeros, value);
  }
};


template <typename SqrtMethod = LUTSqrt<>, typename S, int F, typename I>
constexpr FixedPoint<S, F, I> Sqrt(const FixedPoint<S, F, I> val)
{
  using FP = FixedPoint<S, F, I>;
  const typename FP::StorageType storage = val.Storage();
  if (storage < 0)
    throw std::domain_error("Cannot take square root of a value less than zero");
  else if (storage == 0)
    return FP(0);

  return SqrtMethod::Sqrt(val);
}

} // namespace FPFFT

#endif
