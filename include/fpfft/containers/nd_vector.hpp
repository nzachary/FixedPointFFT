#ifndef FPFFT_CONTAINERS_ND_VECTOR_HPP
#define FPFFT_CONTAINERS_ND_VECTOR_HPP

// Variable size container with any number of dimensions

#include <array>
#include <cstddef>
#include <vector>

namespace FPFFT
{

template <typename T, size_t Dims>
class NDVector
{
 public:

  constexpr NDVector(const std::array<size_t, Dims> dimensions = {}, T fill = T());

  constexpr void Resize(const std::array<size_t, Dims> dimensions, const T fill = T());

  template <typename... P>
  constexpr T& operator()(const P&&... positions);
  template <typename... P>
  constexpr const T& operator()(const P&&... positions) const;

  constexpr T* Data();
  constexpr const T* Data() const;

 private:

  size_t GetIndex(const size_t positions...);

  std::array<size_t, Dims> dimensions;
  size_t elems = 0;
  std::vector<T> data;
};

} // namespace FPFFT

#include "nd_vector_impl.hpp" // IWYU pragma: export

#endif
