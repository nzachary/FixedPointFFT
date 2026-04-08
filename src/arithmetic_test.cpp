#include "test_common.hpp"

// Test basic arithmetic operations on fixed point numbers

using namespace FPFFT;

// Macro to help with creating functions
#define OP_MACRO(operator, name)                                                                   \
  template <typename FP1, typename FP2 = FP1>                                                      \
  double Op##name##FxFx(double a, double b)                                                        \
  {                                                                                                \
    FP1 first(a);                                                                                  \
    FP2 second(b);                                                                                 \
    return double(first operator second);                                                          \
  }                                                                                                \
  template <typename FP>                                                                           \
  double Op##name##FxFl(double a, double b)                                                        \
  {                                                                                                \
    FP first(a);                                                                                   \
    return double(first operator b);                                                               \
  }

OP_MACRO(+, Add)
OP_MACRO(-, Sub)
OP_MACRO(*, Mul)
OP_MACRO(/, Div)

// Macro to run all operations
#define TEST_OPS_T(Type, ...)                                                                      \
  double val;                                                                                      \
                                                                                                   \
  val = OpAdd##Type<__VA_ARGS__>(0, 0);                                                            \
  REQUIRE_APPROX(val, 0);                                                                          \
  val = OpAdd##Type<__VA_ARGS__>(1, 3);                                                            \
  REQUIRE_APPROX(val, 4);                                                                          \
  val = OpAdd##Type<__VA_ARGS__>(2.25, 2.75);                                                      \
  REQUIRE_APPROX(val, 5);                                                                          \
                                                                                                   \
  val = OpSub##Type<__VA_ARGS__>(2.25, 2.75);                                                      \
  REQUIRE_APPROX(val, -0.5);                                                                       \
  val = OpSub##Type<__VA_ARGS__>(2.125, 4.375);                                                    \
  REQUIRE_APPROX(val, -2.25);                                                                      \
  val = OpSub##Type<__VA_ARGS__>(5, 2.5);                                                          \
  REQUIRE_APPROX(val, 2.5);                                                                        \
                                                                                                   \
  val = OpMul##Type<__VA_ARGS__>(2.25, 2);                                                         \
  REQUIRE_APPROX(val, 4.5);                                                                        \
  val = OpMul##Type<__VA_ARGS__>(1, -1);                                                           \
  REQUIRE_APPROX(val, -1);                                                                         \
  val = OpMul##Type<__VA_ARGS__>(4, 1.25);                                                         \
  REQUIRE_APPROX(val, 5);                                                                          \
  val = OpMul##Type<__VA_ARGS__>(-5, 1.25);                                                        \
  REQUIRE_APPROX(val, -6.25);                                                                      \
                                                                                                   \
  val = OpDiv##Type<__VA_ARGS__>(4, 2);                                                            \
  REQUIRE_APPROX(val, 2);                                                                          \
  val = OpDiv##Type<__VA_ARGS__>(6, -2);                                                           \
  REQUIRE_APPROX(val, -3);                                                                         \
  val = OpDiv##Type<__VA_ARGS__>(6, 4);                                                            \
  REQUIRE_APPROX(val, 1.5);                                                                        \
  val = OpDiv##Type<__VA_ARGS__>(-5, -1);                                                          \
  REQUIRE_APPROX(val, 5);

// Macros to run fixed-fixed and fixed-floating tests
#define TEST_OPS_FXFX(...) TEST_OPS_T(FxFx, __VA_ARGS__)
#define TEST_OPS_FXFL(...) TEST_OPS_T(FxFl, __VA_ARGS__)


// Test operations on 2 16-bit fixed point numbers
BEGIN_TEST(FxFx16Test)
  using Fixed16 = FixedPoint<int16_t, 12, int32_t>;
  TEST_OPS_FXFX(Fixed16);
END_TEST

// Test operations on 2 32-bit fixed point numbers
BEGIN_TEST(FxFx32Test)
  using Fixed32 = FixedPoint<int32_t, 28, int64_t>;
  TEST_OPS_FXFX(Fixed32);
END_TEST

// Test operations on 2 32-bit fixed point numbers with different fraction bits
// TODO: Use the higher precision type for calculations instead of the one on the
// left size of the operation
/*
BEGIN_TEST(FxFx3232MixedFracTest)
  using Fixed1 = FixedPoint<int32_t, 22, int64_t>;
  using Fixed2 = FixedPoint<int32_t, 28, int64_t>;
  TEST_OPS(Fixed1, Fixed2);
END_TEST
*/

// Test operations on 16-bit fixed point numbers on 32-bit numbers
BEGIN_TEST(FxFx3216Test)
  using Fixed16 = FixedPoint<int16_t, 12, int32_t>;
  using Fixed32 = FixedPoint<int32_t, 28, int64_t>;
  TEST_OPS_FXFX(Fixed32, Fixed16);
END_TEST

// Test operations on 32-bit fixed point numbers on 16-bit numbers
/*
BEGIN_TEST(FxFx1632Test)
  using Fixed16 = FixedPoint<int16_t, 12, int32_t>;
  using Fixed32 = FixedPoint<int32_t, 28, int64_t>;
  TEST_OPS(Fixed16, Fixed32);
END_TEST
*/

// Test operations on 32-bit fixed point numbers on 16-bit numbers
BEGIN_TEST(FxFx1632Test)
  using Fixed16 = FixedPoint<int16_t, 12, int32_t>;
  using Fixed32 = FixedPoint<int32_t, 28, int64_t>;
  TEST_OPS_FXFX(Fixed16, Fixed32);
END_TEST

// Test operations on a 16-bit fixed point number and a floating point number
BEGIN_TEST(FxFl16Test)
  using Fixed16 = FixedPoint<int16_t, 12, int32_t>;
  TEST_OPS_FXFL(Fixed16);
END_TEST

// Test operations on a 16-bit fixed point number and a floating point number
BEGIN_TEST(FxFl32Test)
  using Fixed32 = FixedPoint<int32_t, 28, int64_t>;
  TEST_OPS_FXFL(Fixed32);
END_TEST


int main()
{
  CALL_TEST(FxFx16Test);
  CALL_TEST(FxFx32Test);
  // CALL_TEST(FxFx3232MixedFracTest);
  CALL_TEST(FxFx3216Test);
  // CALL_TEST(FxFx1632Test);

  CALL_TEST(FxFl16Test);
  CALL_TEST(FxFl32Test);
}
