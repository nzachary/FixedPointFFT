#include "test_common.hpp"

// Test operations on complex numbers

#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace FPFFT;

struct Sine
{
  double amplitude;
  double frequency; // cycles per sample
  double phase;
};

// Execute a FFT on a DC signal
template <size_t Size, typename FP = FixedPoint<int32_t, 16, int64_t>>
NDArray<Complex<FP>, Size> ExecuteFFTDC(double signal)
{
  auto plan = PlanCreator::PlanDFTC2C<Size, FP>();

  for (size_t i = 0; i < Size; i++)
    plan.Input()(i) = Complex<FP>(signal, 0.0);

  plan.Execute();

  return plan.Output();
}

// Execute a FFT on a more complex signal
template <size_t Size, typename FP = FixedPoint<int32_t, 16, int64_t>>
NDArray<Complex<FP>, Size> ExecuteFFTSines(std::vector<Sine> sines)
{
  auto plan = PlanCreator::PlanDFTC2C<Size, FP>();

  for (size_t i = 0; i < Size; i++)
  {
    double signal = 0;
    for (const Sine& sine : sines)
      signal += sine.amplitude * std::sin(2 * M_PI * sine.frequency * i + sine.phase);
    plan.Input()(i) = Complex<FP>(signal, 0.0);
  }

  plan.Execute();

  return plan.Output();
}

// Test the FFT of a DC signal with a size that is a power of 2
BEGIN_TEST(DCSize128FFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double signal = double(rand()) / RAND_MAX;
    auto output = ExecuteFFTDC<128>(signal);
    // The output should have the `Size * signal` in the first bin (0 Hz) and 0 elsewhere
    for (size_t j = 0; j < output.Size(); j++)
    {
      double expect = (j == 0) ? signal * output.Size() : 0;
      REQUIRE_APPROX(double(output(j).Magnitude()), expect, output.Size() * 1e-2);
    }
  }
END_TEST

// Test the FFT of a DC signal with a size that is not a power of 2
BEGIN_TEST(DCSize80FFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double signal = double(rand()) / RAND_MAX;
    auto output = ExecuteFFTDC<80>(signal);
    // The output should have the `Size * signal` in the first bin (0 Hz) and 0 elsewhere
    for (size_t j = 0; j < output.Size(); j++)
    {
      double expect = (j == 0) ? signal * output.Size() : 0;
      REQUIRE_APPROX(double(output(j).Magnitude()), expect, output.Size() * 1e-2);
    }
  }
END_TEST

// Test the FFT on a single sine singal with a size that is a power of 2
BEGIN_TEST(SingleSineSize128FFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double amplitude = double(rand()) / RAND_MAX;
    auto output =
        ExecuteFFTSines<128>({Sine{.amplitude = amplitude, .frequency = 0.25, .phase = 0}});
    // The output should have `Size * amplitude / 2` in the bin corresponding to 0.25 cycles per
    // sample and 0 elsewhere. The / 2 is because the result is mirrored across the halfway point.
    // The bins to expect these spikes are at size * 0.25 and size * (1-0.25).
    for (size_t j = 0; j < output.Size(); j++)
    {
      double expect = (j == output.Size() * 0.25 || j == output.Size() * 0.75) ?
                          amplitude * output.Size() / 2 :
                          0;
      REQUIRE_APPROX(double(output(j).Magnitude()), expect, output.Size() * 1e-2);
    }
  }
END_TEST

// Test the FFT on a single sine singal with a size that is not a power of 2
BEGIN_TEST(SingleSineSize80FFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double amplitude = double(rand()) / RAND_MAX;
    auto output =
        ExecuteFFTSines<80>({Sine{.amplitude = amplitude, .frequency = 0.25, .phase = 0}});
    // The output should have `Size * amplitude / 2` in the bins corresponding to 0.25 cycles per
    // sample and 0 elsewhere. The / 2 is because the result is mirrored across the halfway point.
    // The bins to expect these spikes are at size * 0.25 and size * (1-0.25).
    for (size_t j = 0; j < output.Size(); j++)
    {
      double expect = (j == output.Size() * 0.25 || j == output.Size() * 0.75) ?
                          amplitude * output.Size() / 2 :
                          0;
      REQUIRE_APPROX(double(output(j).Magnitude()), expect, output.Size() * 1e-2);
    }
  }
END_TEST

// Test the FFT on a multi sine singal with a size that is a power of 2
BEGIN_TEST(MultiSineSize128FFTTest)
  for (size_t i = 0; i < 1000; i++)
  {
    double a1 = double(rand()) / RAND_MAX;
    double p1 = 2 * M_PI * double(rand()) / RAND_MAX;
    double a2 = double(rand()) / RAND_MAX;
    double p2 = 2 * M_PI * double(rand()) / RAND_MAX;
    double a3 = double(rand()) / RAND_MAX;
    double p3 = 2 * M_PI * double(rand()) / RAND_MAX;
    auto output = ExecuteFFTSines<128>({
        Sine{.amplitude = a1, .frequency = 0, .phase = p1},    // DC signal
        Sine{.amplitude = a2, .frequency = 0.25, .phase = p2}, // Sine at 0.25 frequency
        Sine{.amplitude = a3, .frequency = 0.125, .phase = p3} // Sine at 0.125 frequency
    });
    // The output should have `Size * amplitude / 2` in the bin corresponding to their frequency and
    // 0 elsewhere. The / 2 is because the result is mirrored across the halfway point. The bins to
    // expect these spikes are at size * (frequency) and size * (1-frequency).
    for (size_t j = 0; j < output.Size(); j++)
    {
      double s1 = (j == 0) ? std::abs(a1 * sin(p1) * output.Size()) : 0; // DC signal
      double s2 = (j == output.Size() * 0.25 || j == output.Size() * 0.75) ?
                      a2 * output.Size() / 2 :
                      0; // Sine at 0.25 frequency
      double s3 = (j == output.Size() * 0.125 || j == output.Size() * 0.875) ?
                      a3 * output.Size() / 2 :
                      0; // Sine at 0.125 frequency
      double expect = s1 + s2 + s3;
      REQUIRE_APPROX(double(output(j).Magnitude()), expect, output.Size() * 1e-2);
    }
  }
END_TEST

// Test the FFT on a multi sine singal with a size that is not a power of 2
BEGIN_TEST(MultiSineSize80FFTTest)
  {
    double a1 = double(rand()) / RAND_MAX;
    double p1 = 2 * M_PI * double(rand()) / RAND_MAX;
    double a2 = double(rand()) / RAND_MAX;
    double p2 = 2 * M_PI * double(rand()) / RAND_MAX;
    double a3 = double(rand()) / RAND_MAX;
    double p3 = 2 * M_PI * double(rand()) / RAND_MAX;
    auto output = ExecuteFFTSines<80>({
        Sine{.amplitude = a1, .frequency = 0, .phase = p1},    // DC signal
        Sine{.amplitude = a2, .frequency = 0.25, .phase = p2}, // Sine at 0.25 frequency
        Sine{.amplitude = a3, .frequency = 0.125, .phase = p3} // Sine at 0.125 frequency
    });
    // The output should have `Size * amplitude / 2` in the bin corresponding to their frequency and
    // 0 elsewhere. The / 2 is because the result is mirrored across the halfway point. The bins to
    // expect these spikes are at size * (frequency) and size * (1-frequency).
    for (size_t j = 0; j < output.Size(); j++)
    {
      double s1 = (j == 0) ? std::abs(a1 * sin(p1) * output.Size()) : 0; // DC signal
      double s2 = (j == output.Size() * 0.25 || j == output.Size() * 0.75) ?
                      a2 * output.Size() / 2 :
                      0; // Sine at 0.25 frequency
      double s3 = (j == output.Size() * 0.125 || j == output.Size() * 0.875) ?
                      a3 * output.Size() / 2 :
                      0; // Sine at 0.125 frequency
      double expect = s1 + s2 + s3;
      REQUIRE_APPROX(double(output(j).Magnitude()), expect, output.Size() * 1e-2);
    }
  }
END_TEST


int main()
{
  CALL_TEST(DCSize128FFTTest);
  CALL_TEST(DCSize80FFTTest);
  CALL_TEST(SingleSineSize128FFTTest);
  CALL_TEST(SingleSineSize80FFTTest);
  CALL_TEST(MultiSineSize128FFTTest);
  CALL_TEST(MultiSineSize80FFTTest);
}
