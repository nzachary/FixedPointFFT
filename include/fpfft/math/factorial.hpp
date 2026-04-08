#ifndef FPFFT_MATH_FACTORIAL_HPP
#define FPFFT_MATH_FACTORIAL_HPP

namespace FPFFT
{

template <typename T>
T Factorial(const T val)
{
  T product = 1;
  for (T i = 2; i <= val; i++)
    product *= i;
  return product;
}

} // namespace FPFFT

#endif
