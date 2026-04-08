#ifndef FPFFT_CONTAINERS_LUT_HPP
#define FPFFT_CONTAINERS_LUT_HPP

// Base class to help with creating a lookup table

#include "nd_array.hpp"

namespace FPFFT
{

template <typename T, size_t X, size_t Y = 0, size_t Z = 0>
class LUT
{
 public:

  constexpr LUT() :
      data()
  {
  }

  constexpr const T& operator()(size_t i) const
  {
    return data(i);
  }
  constexpr const T& operator()(size_t x, size_t y) const
  {
    return data(x, y);
  }
  constexpr const T& operator()(size_t x, size_t y, const size_t z) const
  {
    return data(x, y, z);
  }

 protected:

  NDArray<T, X, Y, Z> data;
};

} // namespace FPFFT

#endif
