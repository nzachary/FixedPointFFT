#include "test_common.hpp"

// Test more complex math functions like sqrt and pow

#include "../include/fixedpointfft.hpp"
#include <cmath>
#include <cstdint>
#include <cstdlib>
using namespace FPFFT;

BEGIN_TEST(Sqrt32Test)
  using FP = FixedPoint<int32_t, 28, int64_t>;

  FP out;
  for (size_t i = 0; i < 10000; i++)
  {
    double x = 4 * (double)rand() / RAND_MAX;
    double y = sqrt(x);
    FP fx(x);

    out = Sqrt<HeronsSqrt<>>(fx);
    REQUIRE_APPROX_EPS(double(out), y, 1e-2);
    out = Sqrt<BakhshaliSqrt<>>(fx);
    REQUIRE_APPROX_EPS(double(out), y, 1e-2);
    out = Sqrt<LUTSqrt<>>(fx);
    REQUIRE_APPROX_EPS(double(out), y, 1e-2);
  }
END_TEST

BEGIN_TEST(Pow32Test)
  using FP = FixedPoint<int32_t, 28, int64_t>;

  FP out;
  for (size_t i = 0; i < 10000; i++)
  {
    double x = 2 * (double)rand() / RAND_MAX;
    double n = std::floor(4 * (double)rand() / RAND_MAX);
    double y = pow(x, n);
    FP fx(x);

    out = PowI(fx, int(n));
    REQUIRE_APPROX_EPS(double(out), y, 1e-2);
  }
END_TEST

BEGIN_TEST(Exp32Test)
  using FP = FixedPoint<int32_t, 24, int64_t>;

  FP out;
  for (size_t i = 0; i < 10000; i++)
  {
    double x = 2 * (double)rand() / RAND_MAX;
    double y = exp(x);
    FP fx(x);

    out = Exp(fx);
    REQUIRE_APPROX_EPS(double(out), y, 1e-2);
  }
END_TEST

BEGIN_TEST(SinCos32Test)
  using FP = FixedPoint<int32_t, 28, int64_t>;

  FP out;
  for (size_t i = 0; i < 10000; i++)
  {
    double x = 4 * double(rand()) / RAND_MAX;
    double s = sin(x * 2 * M_PI);
    double c = cos(x * 2 * M_PI);
    FP fx(x);

    out = Sin2PI(fx);
    REQUIRE_APPROX_EPS(double(out), s, 1e-2);
    out = Cos2PI(fx);
    REQUIRE_APPROX_EPS(double(out), c, 1e-2);
  }
END_TEST

int main()
{
  CALL_TEST(Sqrt32Test);
  CALL_TEST(Pow32Test);
  CALL_TEST(Exp32Test);
  CALL_TEST(SinCos32Test);
}
