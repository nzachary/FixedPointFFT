#ifndef FPFFT_PLAN_IMPL_HPP
#define FPFFT_PLAN_IMPL_HPP

#include "fpfft/debug/assert.hpp"
#include "fpfft/numbers/sfinae.hpp"
#include "fpfft/transforms/dft_naive.hpp"
#include "fpfft/transforms/fft_cooleytukey.hpp"
#include "plan.hpp"
#include <stdexcept>

namespace FPFFT
{

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
Plan<InType, OutType, InSize, OutSize>::Plan()
{
}

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
NDArray<InType, InSize>& Plan<InType, OutType, InSize, OutSize>::Input()
{
  return input;
}

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
const NDArray<InType, InSize>& Plan<InType, OutType, InSize, OutSize>::Input() const
{
  return input;
}

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
NDArray<OutType, OutSize>& Plan<InType, OutType, InSize, OutSize>::Output()
{
  return output;
}

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
const NDArray<OutType, OutSize>& Plan<InType, OutType, InSize, OutSize>::Output() const
{
  return output;
}

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
void Plan<InType, OutType, InSize, OutSize>::Execute()
{
  constexpr bool InComplex = IsComplex<InType>::value;
  constexpr bool OutComplex = IsComplex<OutType>::value;

  for (const Step& s : steps)
  {
    const InType* inPtr = input.Data() + s.inOffset;
    OutType* outPtr = output.Data() + s.outOffset;
    switch (s.func)
    {
      case Step::Function::COPY:
        {
          FPFFT_ASSERT(s.inSize == s.outSize);
          for (size_t i = 0; i < s.inSize; i++)
            *outPtr++ = *inPtr++;
        }
        break;
      case Step::Function::DFTNaive:
        {
          FPFFT_ASSERT(
              InComplex && OutComplex, "Other types of transforms have not been implemented yet");
          if constexpr (InComplex && OutComplex)
            DFTNaiveC2C(inPtr, outPtr, s.inSize);
        }
        break;
      case Step::Function::FFTCooleyTukeyDepthFirstRadix2:
        {
          FPFFT_ASSERT(
              InComplex && OutComplex, "Other types of transforms have not been implemented yet");
          if constexpr (InComplex && OutComplex)
            FFTCooleyTukeyDepthFirstC2C(inPtr, outPtr, s.inSize, 1);
        }
        break;
      default:
        throw std::logic_error("Unimplemented function");
        break;
    }
  }
}

} // namespace FPFFT

#endif
