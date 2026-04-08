#ifndef FPFFT_NUMBERS_FIXED_POINT_IMPL_HPP
#define FPFFT_NUMBERS_FIXED_POINT_IMPL_HPP

#include "fixed_point.hpp"
#include <cmath> // IWYU pragma: keep

namespace FPFFT
{

template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I>::FixedPoint() :
    storage(0)
{
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I>::FixedPoint(FromType from) :
    storage(from * Scale)
{
}

template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::PI()
{
  return FixedPoint(M_PI);
}

template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::E()
{
  return FixedPoint(M_E);
}

template <typename S, int F, typename I>
constexpr bool FixedPoint<S, F, I>::IsIntegral() const
{
  return storage % Scale == 0;
}

template <typename S, int F, typename I>
constexpr S FixedPoint<S, F, I>::Integer() const
{
  return storage / Scale;
}

template <typename S, int F, typename I>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::Fraction() const
{
  FixedPoint<S, F, I> out;
  out.Storage() = storage % Scale;
  return out;
}

template <typename S, int F, typename I>
template <typename FromType, std::enable_if_t<!IsFixed<FromType>::value>>
constexpr void FixedPoint<S, F, I>::operator=(FromType from)
{
  storage = from * Scale;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr void FixedPoint<S, F, I>::operator=(FixedPoint<OS, OF, OI> from)
{
  storage = S(from.storage) >> (F - OF);
}

template <typename S, int F, typename I>
constexpr const S& FixedPoint<S, F, I>::Storage() const
{
  return storage;
}

template <typename S, int F, typename I>
constexpr S& FixedPoint<S, F, I>::Storage()
{
  return storage;
}

template <typename S, int F, typename I>
template <typename ConvertType>
constexpr FixedPoint<S, F, I>::operator ConvertType() const
{
  return ConvertType(storage) / Scale;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator+(FromType other) const
{
  FixedPoint out;
  out.storage = storage + other * Scale;
  return out;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator+=(FromType other)
{
  storage += other * Scale;
  return *this;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator+(FixedPoint<OS, OF, OI> other) const
{
  FixedPoint out;
  out.storage = storage + (S(other.Storage()) << (F - OF));
  return out;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator+=(FixedPoint<OS, OF, OI> other)
{
  storage += S(other.Storage()) << (F - OF);
  return *this;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator-(FromType other) const
{
  FixedPoint out;
  out.storage = storage - other * Scale;
  return out;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator-=(FromType other)
{
  storage -= other * Scale;
  return *this;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator-(FixedPoint<OS, OF, OI> other) const
{
  FixedPoint out;
  out.storage = storage - (S(other.Storage()) << (F - OF));
  return out;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator-=(FixedPoint<OS, OF, OI> other)
{
  storage -= S(other.Storage()) << (F - OF);
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator*(FromType other) const
{
  FixedPoint out;
  out.storage = storage * other;
  return out;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator*=(FromType other)
{
  storage *= other;
  return *this;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator*(FixedPoint<OS, OF, OI> other) const
{
  FixedPoint out;
  I intermediate = I(storage) * other.Storage();
  out.storage = intermediate >> OF;
  return out;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator*=(FixedPoint<OS, OF, OI> other)
{
  I intermediate = I(storage) * other.Storage();
  storage = intermediate >> OF;
  return *this;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator/(FromType other) const
{
  FixedPoint out;
  out.storage = storage / other;
  return out;
}

template <typename S, int F, typename I>
template <typename FromType>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator/=(FromType other)
{
  storage /= other;
  return *this;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I> FixedPoint<S, F, I>::operator/(FixedPoint<OS, OF, OI> other) const
{
  FixedPoint out;
  constexpr int maxScaleBits = (sizeof(I) - sizeof(S)) * CHAR_BIT;
  I intermediate = I(storage) << (maxScaleBits + OF - F);
  intermediate /= other.Storage();
  out.storage = intermediate >> (maxScaleBits - F);
  return out;
}

template <typename S, int F, typename I>
template <typename OS, int OF, typename OI>
constexpr FixedPoint<S, F, I>& FixedPoint<S, F, I>::operator/=(FixedPoint<OS, OF, OI> other)
{
  constexpr int maxScaleBits = (sizeof(I) - sizeof(S)) * CHAR_BIT;
  I intermediate = I(storage) << (maxScaleBits + OF - F);
  intermediate /= other.Storage();
  storage = intermediate >> (maxScaleBits - F);
}

} // namespace FPFFT

#endif
