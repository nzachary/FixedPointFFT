#ifndef FPFFT_CONTAINERS_ND_VECTOR_IMPL_HPP
#define FPFFT_CONTAINERS_ND_VECTOR_IMPL_HPP

#include "fpfft/debug/assert.hpp" // IWYU pragma: keep
#include "nd_vector.hpp"
#include <cstdarg>

namespace FPFFT
{

template <typename T, size_t Dims>
constexpr NDVector<T, Dims>::NDVector(const std::array<size_t, Dims> dimensions, T fill)
{
  Resize(dimensions, fill);
}

template <typename T, size_t Dims>
constexpr void NDVector<T, Dims>::Resize(const std::array<size_t, Dims> dimensions, const T fill)
{
  this->dimensions = dimensions;
  elems = 1;
  for (size_t dim : dimensions)
    elems *= dim;
  data.resize(elems);
}

template <typename T, size_t Dims>
template <typename... P>
constexpr T& NDVector<T, Dims>::operator()(const P&&... positions)
{
  return data[GetIndex(positions...)];
}

template <typename T, size_t Dims>
template <typename... P>
constexpr const T& NDVector<T, Dims>::operator()(const P&&... positions) const
{
  return data[GetIndex(positions...)];
}

template <typename T, size_t Dims>
constexpr T* NDVector<T, Dims>::Data()
{
  return data.data();
}

template <typename T, size_t Dims>
constexpr const T* NDVector<T, Dims>::Data() const
{
  return data.data();
}

template <typename T, size_t Dims>
size_t NDVector<T, Dims>::GetIndex(const size_t positions...)
{
  std::va_list args;
  va_start(args, positions);
  size_t index = 0;
  size_t multiplier = 1;
  for (int i = 0; i < positions; i++)
  {
    const size_t pos = va_arg(args, size_t);
    const size_t dim = dimensions[i];
    FPFFT_ASSERT(pos < dim, std::invalid_argument, "Position is out of range");
    index += pos * multiplier;
    multiplier *= dim;
  }
}

} // namespace FPFFT

#endif
