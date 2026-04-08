#ifndef FPFFT_CONTAINERS_ND_ARRAY_HPP
#define FPFFT_CONTAINERS_ND_ARRAY_HPP

// Fixed size container with N dimensions

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

 private:

  static constexpr size_t Size = X * std::max(Y, size_t(1)) * std::max(Z, size_t(1));
  std::array<T, Size> data;
};

} // namespace FPFFT

#endif
