#include "test_common.hpp"

// Test inverse FFT functions

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace FPFFT;

struct Bin
{
  size_t index;
  double r;
  double c;
};

// Execute an IFFT
template <size_t Size, typename FP = FixedPoint<int32_t, 16, int64_t>>
NDArray<Complex<FP>, Size> ExecuteIFFT(std::vector<Bin> bins)
{
  auto plan = PlanCreator::PlanIDFTC2C<Size, FP>();

  for (const Bin& b : bins)
    plan.Input()(b.index) = Complex<FP>(b.r, b.c);

  plan.Execute();

  return NDArray<Complex<FP>, Size>(plan.Output());
}

// Test the IFFT of a DC signal with a size that is a power of 2
BEGIN_TEST(DCSize128IFFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double signal = double(rand()) / RAND_MAX;
    auto output = ExecuteIFFT<128>({Bin{.index = 0, .r = 0, .c = signal * 128}});
    // The output should have `signal` in all bins
    for (size_t j = 0; j < output.Size(); j++)
    {
      REQUIRE_APPROX(double(output(j).Magnitude()), signal, output.Size() * 1e-2);
    }
  }
END_TEST

// Test the IFFT of a DC signal with a size that is not a power of 2
BEGIN_TEST(DCSize80IFFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double signal = double(rand()) / RAND_MAX;
    auto output = ExecuteIFFT<80>({Bin{.index = 0, .r = 0, .c = signal * 80}});
    for (size_t j = 0; j < output.Size(); j++)
    {
      REQUIRE_APPROX(double(output(j).Magnitude()), signal, output.Size() * 1e-2);
    }
  }
END_TEST


int main()
{
  CALL_TEST(DCSize128IFFTTest);
  CALL_TEST(DCSize80IFFTTest);
}
