#ifndef FPFFT_TRANSFORMS_FFT_COOLEYTUKEY_HPP
#define FPFFT_TRANSFORMS_FFT_COOLEYTUKEY_HPP

#include "fpfft/debug/assert.hpp"
#include "fpfft/math/pow.hpp"
#include "fpfft/numbers/sfinae.hpp"
#include <cstddef>
#include <type_traits>
#include <vector>

namespace FPFFT
{

// Recursive implementation of the Cooley-Tukey algorithm
template <typename Complex, std::enable_if_t<IsComplex<Complex>::value, bool> = false>
void FFTCooleyTukeyDepthFirstC2C(
    const Complex* in, Complex* out, const size_t N, const size_t stride)
{
  using FP = typename Complex::FixedType;

  if (N == 1)
  {
    out[0] = in[0];
  }
  else
  {
    const size_t halfN = N / 2;
    FFTCooleyTukeyDepthFirstC2C(in, out, halfN, 2 * stride);
    FFTCooleyTukeyDepthFirstC2C(in + stride, out + halfN, halfN, 2 * stride);

    FP x;
    Complex q;
    for (size_t k = 0; k < halfN; k++)
    {
      x.Storage() = -(k * FP::Scale) / N;

      // p = X_k
      // q = x_k+N/2
      // Y_k = p + q
      // Y_k+N/2 = p - q
      q = Exp2PIC(x) * out[k + halfN];
      out[k + halfN] = out[k] - q;
      out[k] += q;
    }
  }
}

} // namespace FPFFT

#endif
