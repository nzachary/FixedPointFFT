#ifndef FPFFT_TRANSFORMS_DFT_NAIVE_HPP
#define FPFFT_TRANSFORMS_DFT_NAIVE_HPP

#include "fpfft/math/pow.hpp"
#include "fpfft/numbers/sfinae.hpp"
#include <cstddef>
#include <type_traits>

namespace FPFFT
{

template <typename Complex, std::enable_if_t<IsComplex<Complex>::value, bool> = false>
void DFTNaiveC2C(const Complex* in, Complex* out, const size_t N)
{
  using FP = typename Complex::FixedType;

  FP x;
  for (size_t i = 0; i < N; ++i)
  {
    out[i] = Complex(0, 0);
    for (size_t j = 0; j < N; ++j)
    {
      x.Storage() = (j * i * FP::Scale) / N;
      Complex term = in[j] * Exp2PIC(x);
      out[i] += term;
    }
  }
}

} // namespace FPFFT

#endif
