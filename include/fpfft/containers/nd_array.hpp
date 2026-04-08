#ifndef FPFFT_CONTAINERS_ND_ARRAY_HPP
#define FPFFT_CONTAINERS_ND_ARRAY_HPP

// Fixed size container with up to 3 dimensions

#include "fpfft/debug/assert.hpp"
#include <array>
#include <cstddef>

namespace FPFFT
{

template <typename T, size_t X, size_t Y = 0, size_t Z = 0>
class NDArray
{
 public:

  constexpr NDArray() :
      data()
  {
  }

  constexpr size_t Size() const
  {
    return TotalSize;
  }

  constexpr T& operator()(size_t i)
  {
    return data[i];
  }
  constexpr const T& operator()(size_t i) const
  {
    return data[i];
  }

  constexpr T& operator()(size_t x, size_t y)
  {
    return data[y * X + x];
  }
  constexpr const T& operator()(size_t x, size_t y) const
  {
    return data[y * X + x];
  }

  constexpr T& operator()(size_t x, size_t y, size_t z)
  {
    return data[z * Y * X + y * X + x];
  }
  constexpr const T& operator()(size_t x, size_t y, size_t z) const
  {
    return data[z * Y * X + y * X + x];
  }

  constexpr T* Data()
  {
    return data.data();
  }

  constexpr const T* Data() const
  {
    return data.data();
  }

 private:

  static constexpr size_t TotalSize = X * std::max(Y, size_t(1)) * std::max(Z, size_t(1));
  std::array<T, TotalSize> data;
};

} // namespace FPFFT

#endif
