#ifndef FPFFT_NUMBERS_COMPLEX_IMPL_HPP
#define FPFFT_NUMBERS_COMPLEX_IMPL_HPP

#include "complex.hpp"
#include "fpfft/math/sqrt.hpp"

namespace FPFFT
{

template <typename FP>
Complex<FP>::Complex() :
    real(0),
    imaginary(0)
{
}

template <typename FP>
template <typename T>
Complex<FP>::Complex(T r, T i) :
    real(r),
    imaginary(i)
{
}

template <typename FP>
FP& Complex<FP>::Real()
{
  return real;
}

template <typename FP>
const FP& Complex<FP>::Real() const
{
  return real;
}

template <typename FP>
FP& Complex<FP>::Imaginary()
{
  return imaginary;
}

template <typename FP>
const FP& Complex<FP>::Imaginary() const
{
  return imaginary;
}

template <typename FP>
Complex<FP> Complex<FP>::Conjugated() const
{
  return Complex(real, -imaginary);
}

template <typename FP>
Complex<FP>& Complex<FP>::Conjugate()
{
  imaginary = -imaginary;
  return *this;
}

template <typename FP>
FP Complex<FP>::Magnitude() const
{
  return Sqrt(real * real + imaginary * imaginary);
}

template <typename FP>
Complex<FP> Complex<FP>::operator+(Complex other) const
{
  return Complex(real + other.real, imaginary + other.imaginary);
}

template <typename FP>
Complex<FP>& Complex<FP>::operator+=(Complex other)
{
  real += other.real;
  imaginary += other.imaginary;
  return *this;
}

template <typename FP>
Complex<FP> Complex<FP>::operator-(Complex other) const
{
  return Complex(real - other.real, imaginary - other.imaginary);
}

template <typename FP>
Complex<FP>& Complex<FP>::operator-=(Complex other)
{
  real -= other.real;
  imaginary -= other.imaginary;
  return *this;
}

template <typename FP>
Complex<FP> Complex<FP>::operator*(Complex other) const
{
  return Complex(real * other.real - imaginary * other.imaginary,
      real * other.imaginary + imaginary * other.real);
}

template <typename FP>
Complex<FP>& Complex<FP>::operator*=(Complex other)
{
  real = real * other.real - imaginary * other.imaginary;
  imaginary = real * other.imaginary + imaginary * other.real;
  return *this;
}

template <typename FP>
Complex<FP> Complex<FP>::operator/(Complex other) const
{
  // The common denominator is the magnitude of `other` squared
  const FP denominator = other.real * other.real + other.imaginary * other.imaginary;
  return Complex((real * other.real + imaginary * other.imaginary) / denominator,
      (imaginary * other.real - real * other.imaginary) / denominator);
}

template <typename FP>
Complex<FP>& Complex<FP>::operator/=(Complex other)
{
  // The common denominator is the magnitude of `other` squared
  const FP denominator = other.real * other.real + other.imaginary * other.imaginary;
  real = (real * other.real + imaginary * other.imaginary) / denominator;
  imaginary = (imaginary * other.real - real * other.imaginary) / denominator;
  return *this;
}


} // namespace FPFFT

#endif
