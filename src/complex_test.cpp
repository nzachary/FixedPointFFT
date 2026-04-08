#include "test_common.hpp"

// Test operations on complex numbers

using namespace FPFFT;

#define TEST_ADD                                                                                   \
  C out;                                                                                           \
                                                                                                   \
  out = C(1, 1) + C(0, 0);                                                                         \
  REQUIRE_APPROX(double(out.Real()), 1);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 1);                                                      \
                                                                                                   \
  out = C(1, 1) + C(-1, -1);                                                                       \
  REQUIRE_APPROX(double(out.Real()), 0);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 0);                                                      \
                                                                                                   \
  out = C(5, 3) + C(-3, -1);                                                                       \
  REQUIRE_APPROX(double(out.Real()), 2);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 2);                                                      \
                                                                                                   \
  out = C(-1.0, 0.5) + C(-3, -3);                                                                  \
  REQUIRE_APPROX(double(out.Real()), -4);                                                          \
  REQUIRE_APPROX(double(out.Imaginary()), -2.5);                                                   \
                                                                                                   \
  out = C(-0.75, 1.25) + C(-0.25, 0.75);                                                           \
  REQUIRE_APPROX(double(out.Real()), -1);                                                          \
  REQUIRE_APPROX(double(out.Imaginary()), 2);

#define TEST_SUB                                                                                   \
  C out;                                                                                           \
                                                                                                   \
  out = C(1, 1) - C(0, 0);                                                                         \
  REQUIRE_APPROX(double(out.Real()), 1);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 1);                                                      \
                                                                                                   \
  out = C(1, 1) - C(-1, -1);                                                                       \
  REQUIRE_APPROX(double(out.Real()), 2);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 2);                                                      \
                                                                                                   \
  out = C(5, 3) - C(3, -1);                                                                        \
  REQUIRE_APPROX(double(out.Real()), 2);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 4);                                                      \
                                                                                                   \
  out = C(-1.0, 0.5) - C(-3, -3);                                                                  \
  REQUIRE_APPROX(double(out.Real()), 2);                                                           \
  REQUIRE_APPROX(double(out.Imaginary()), 3.5);                                                    \
                                                                                                   \
  out = C(-0.75, 1.25) - C(-0.25, 0.75);                                                           \
  REQUIRE_APPROX(double(out.Real()), -0.5);                                                        \
  REQUIRE_APPROX(double(out.Imaginary()), 0.5);


// Test adding 2 16-bit based complex numbers
BEGIN_TEST(Add16Test)
  using FP = FixedPoint<int16_t, 12, int32_t>;
  using C = Complex<FP>;
  TEST_ADD;
END_TEST

// Test adding 2 32-bit based complex numbers
BEGIN_TEST(Add32Test)
  using FP = FixedPoint<int32_t, 28, int64_t>;
  using C = Complex<FP>;
  TEST_ADD;
END_TEST

// Test subtracting 2 16-bit based complex numbers
BEGIN_TEST(Sub16Test)
  using FP = FixedPoint<int16_t, 12, int32_t>;
  using C = Complex<FP>;
  TEST_SUB;
END_TEST

// Test subtracting 2 32-bit based complex numbers
BEGIN_TEST(Sub32Test)
  using FP = FixedPoint<int32_t, 28, int64_t>;
  using C = Complex<FP>;
  TEST_SUB;
END_TEST

// TODO: other tests


int main()
{
  CALL_TEST(Add16Test);
  CALL_TEST(Add32Test);
  CALL_TEST(Sub16Test);
  CALL_TEST(Sub32Test);
}
