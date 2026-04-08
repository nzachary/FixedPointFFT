#ifndef FPFFT_NUMBERS_FIXED_POINT_HPP
#define FPFFT_NUMBERS_FIXED_POINT_HPP

#include "sfinae.hpp"
#include <climits>
#include <type_traits>

namespace FPFFT
{

// S: Storage type
// F: Number of bits for the fraction
// I: Intermediate type
template <typename S, int F, typename I /* = S */>
class FixedPoint
{
 public:

  using StorageType = S;
  static constexpr int FractionBits = F;
  using IntermediateType = I;
  static constexpr I Scale = S(1) << F;

  constexpr FixedPoint();

  // Constants
  static constexpr FixedPoint PI();
  static constexpr FixedPoint E();

  // Checks if the number is integral; Is a whole number
  constexpr bool IsIntegral() const;
  // Gets the integer component of the number; Left size of the decimal point
  constexpr S Integer() const;
  // Gets the fraction component of the number; Right side of the decimal point
  constexpr FixedPoint Fraction() const;

  // Convert from another type to a fixed point type
  template <typename FromType>
  constexpr FixedPoint(FromType from);
  template <typename FromType, std::enable_if_t<!IsFixed<FromType>::value>>
  constexpr void operator=(FromType from);
  // Convert from one fixed point type to another
  template <typename OS, int OF, typename OI>
  constexpr void operator=(FixedPoint<OS, OF, OI> from);

  constexpr const S& Storage() const;
  constexpr S& Storage();

  // Convert from a fixed point nubmer to another type
  template <typename ConvertType>
  constexpr operator ConvertType() const;


  // Basic arithmetic operations
  template <typename FromType>
  constexpr FixedPoint operator+(FromType other) const;
  template <typename FromType>
  constexpr FixedPoint& operator+=(FromType other);
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint operator+(FixedPoint<OS, OF, OI> other) const;
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint& operator+=(FixedPoint<OS, OF, OI> other);

  template <typename FromType>
  constexpr FixedPoint operator-(FromType other) const;
  template <typename FromType>
  constexpr FixedPoint& operator-=(FromType other);
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint operator-(FixedPoint<OS, OF, OI> other) const;
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint& operator-=(FixedPoint<OS, OF, OI> other);

  template <typename FromType>
  constexpr FixedPoint operator*(FromType other) const;
  template <typename FromType>
  constexpr FixedPoint& operator*=(FromType other);
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint operator*(FixedPoint<OS, OF, OI> other) const;
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint& operator*=(FixedPoint<OS, OF, OI> other);

  template <typename FromType>
  constexpr FixedPoint operator/(FromType other) const;
  template <typename FromType>
  constexpr FixedPoint& operator/=(FromType other);
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint operator/(FixedPoint<OS, OF, OI> other) const;
  template <typename OS, int OF, typename OI>
  constexpr FixedPoint& operator/=(FixedPoint<OS, OF, OI> other);

 private:

  S storage = 0;

  static_assert(std::is_signed_v<S>);
  static_assert(std::is_integral_v<S>);
  static_assert(F < sizeof(S) * CHAR_BIT);
  static_assert(F > 0);

  static_assert(std::is_signed_v<I>);
  static_assert(std::is_integral_v<I>);
  static_assert(sizeof(I) >= sizeof(S));
};

} // namespace FPFFT

#include "fixed_point_impl.hpp" // IWYU pragma: export

#endif
