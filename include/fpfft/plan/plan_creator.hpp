#ifndef FPFFT_PLAN_CREATOR_HPP
#define FPFFT_PLAN_CREATOR_HPP

#include "plan.hpp"
#include <type_traits>

namespace FPFFT
{

class PlanCreator
{
 public:

  PlanCreator();

  // Use these functions to create plans
  template <size_t N, typename FixedType, std::enable_if_t<IsFixed<FixedType>::value, bool> = false>
  static Plan<Complex<FixedType>, Complex<FixedType>, N, N> PlanDFTC2C();

  template <size_t N, typename FixedType, std::enable_if_t<IsFixed<FixedType>::value, bool> = false>
  static Plan<Complex<FixedType>, Complex<FixedType>, N, N> PlanIDFTC2C();
};

} // namespace FPFFT

#include "plan_creator_impl.hpp" // IWYU pragma: export

#endif
