#ifndef FPFFT_NUMBERS_COMPLEX_HPP
#define FPFFT_NUMBERS_COMPLEX_HPP

#include "fixed_point.hpp" // IWYU pragma: export
#include "sfinae.hpp"

namespace FPFFT
{

// FP: Fixed point type to use
template <typename FP>
class Complex
{
 public:

  using FixedType = FP;

  Complex();
  template <typename T>
  Complex(T r, T i);

  FP& Real();
  const FP& Real() const;

  FP& Imaginary();
  const FP& Imaginary() const;

  Complex Conjugated() const;
  Complex& Conjugate();

  FP Magnitude() const;


  // Basic arithmetic operations
  Complex operator+(Complex other) const;
  Complex& operator+=(Complex other);

  Complex operator-(Complex other) const;
  Complex& operator-=(Complex other);

  Complex operator*(Complex other) const;
  Complex& operator*=(Complex other);
  template <typename T>
  Complex operator*(T other) const;
  template <typename T>
  Complex& operator*=(T other);

  Complex operator/(Complex other) const;
  Complex& operator/=(Complex other);
  template <typename T>
  Complex operator/(T other) const;
  template <typename T>
  Complex& operator/=(T other);

 private:

  FP real;
  FP imaginary;

  static_assert(IsFixed<FP>::value);
};

} // namespace FPFFT

#include "complex_impl.hpp" // IWYU pragma: export

#endif
