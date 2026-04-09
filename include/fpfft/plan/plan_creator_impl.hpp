#ifndef FPFFT_PLAN_CREATOR_IMPL_HPP
#define FPFFT_PLAN_CREATOR_IMPL_HPP

#include "plan_creator.hpp"

namespace FPFFT
{

template <size_t N, typename FixedType, std::enable_if_t<IsFixed<FixedType>::value, bool>>
Plan<Complex<FixedType>, Complex<FixedType>, N, N> PlanCreator::PlanDFTC2C()
{
  static_assert(N > 0);

  using OutPlanType = Plan<Complex<FixedType>, Complex<FixedType>, N, N>;
  OutPlanType plan;

  constexpr bool isPowerOf2 = (N & (N - 1)) == 0;
  if constexpr (N == 1)
  {
    plan.steps.push_back(typename OutPlanType::Step{
        .func = OutPlanType::Step::Function::COPY,
        .inSize = N,
        .inOffset = 0,
        .outSize = N,
        .outOffset = 0,
    });
  }
  else if constexpr (isPowerOf2)
  {
    plan.steps.push_back(typename OutPlanType::Step{
        .func = OutPlanType::Step::Function::FFTCooleyTukeyDepthFirstRadix2,
        .inSize = N,
        .inOffset = 0,
        .outSize = N,
        .outOffset = 0,
    });
  }
  else
  {
    plan.steps.push_back(typename OutPlanType::Step{
        .func = OutPlanType::Step::Function::DFTNaive,
        .inSize = N,
        .inOffset = 0,
        .outSize = N,
        .outOffset = 0,
    });
  }

  return plan;
}

template <size_t N, typename FixedType, std::enable_if_t<IsFixed<FixedType>::value, bool>>
Plan<Complex<FixedType>, Complex<FixedType>, N, N> PlanCreator::PlanIDFTC2C()
{
  static_assert(N > 0);

  using OutPlanType = Plan<Complex<FixedType>, Complex<FixedType>, N, N>;
  OutPlanType plan;

  constexpr bool isPowerOf2 = (N & (N - 1)) == 0;
  if constexpr (N == 1)
  {
    plan.steps.push_back(typename OutPlanType::Step{
        .func = OutPlanType::Step::Function::COPY,
        .inSize = N,
        .inOffset = 0,
        .outSize = N,
        .outOffset = 0,
    });
  }
  else if constexpr (isPowerOf2)
  {
    plan.steps.push_back(typename OutPlanType::Step{
        .func = OutPlanType::Step::Function::IFFTCooleyTukeyDepthFirstRadix2,
        .inSize = N,
        .inOffset = 0,
        .outSize = N,
        .outOffset = 0,
    });
  }
  else
  {
    plan.steps.push_back(typename OutPlanType::Step{
        .func = OutPlanType::Step::Function::IDFTNaive,
        .inSize = N,
        .inOffset = 0,
        .outSize = N,
        .outOffset = 0,
    });
  }

  return plan;
}

} // namespace FPFFT

#endif
