#ifndef FPFFT_PLAN_HPP
#define FPFFT_PLAN_HPP

#include "fpfft/containers/nd_array.hpp"
#include "fpfft/numbers/complex.hpp" // IWYU pragma: export
#include <vector>

namespace FPFFT
{

template <typename InType, typename OutType, size_t InSize, size_t OutSize>
class Plan
{
 public:

  Plan();

  // Use this class to create plans
  friend class PlanCreator;

  NDArray<InType, InSize>& Input();
  const NDArray<InType, InSize>& Input() const;
  NDArray<OutType, OutSize>& Output();
  const NDArray<OutType, OutSize>& Output() const;

  void Execute();

 private:

  // A single step of a plan
  struct Step
  {
    // Function to call at each step
    enum Function
    {
      COPY,                           // Just a plain copy from the input to output
      DFTNaive,                       // Naive discrete fourier transform
      FFTCooleyTukeyDepthFirstRadix2, // Cooley-Tukey algorithm (depth first), radix 2
      FFTCooleyTukeyRadix2,           // Cooley-Tukey algorithm, radix 2
    } func;

    size_t inSize;
    size_t inOffset;
    size_t outSize;
    size_t outOffset;

    // Optional data that can be passed to the function
    size_t data1;
    size_t data2;
  };

  NDArray<InType, InSize> input;
  NDArray<OutType, OutSize> output;
  std::vector<Step> steps;
};

} // namespace FPFFT

#include "plan_impl.hpp" // IWYU pragma: export

#endif
