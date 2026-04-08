#include "../include/fixedpointfft.hpp"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define QUOTES(v) #v
#define STRINGIFY(v) QUOTES(v)

#define BEGIN_TEST(name)                                                                           \
  void name()                                                                                      \
  {                                                                                                \
    const char* _testname_ = _FPFFT_STRINGIFY(name);                                               \
    printf("Beginning test: %s\n", _testname_);

#define END_TEST                                                                                   \
  printf("Test passed: %s\n", _testname_);                                                         \
  }

#define CALL_TEST(name) name();


void handle_fail()
{
  abort();
}

#define TEST_FAIL(format, ...)                                                                     \
  printf("%s:%d\nTest failed: " format "\n", __FILE__, __LINE__, __VA_ARGS__);                     \
  handle_fail();

#define REQUIRE(cond)                                                                              \
  if (!(cond))                                                                                     \
  {                                                                                                \
    TEST_FAIL("(%s) == false", STRINGIFY(cond));                                                   \
  }

#define REQUIRE_APPROX_EPS(a, b, eps)                                                              \
  {                                                                                                \
    double diff = fabs(a - b);                                                                     \
    if (diff >= eps)                                                                               \
    {                                                                                              \
      TEST_FAIL("%lf != %lf", (double)(a), (double)(b));                                           \
    }                                                                                              \
  }

#define REQUIRE_APPROX(a, b) REQUIRE_APPROX_EPS(a, b, 1e-3)

#define REQUIRE_EQUALS(a, b)                                                                       \
  {                                                                                                \
    if (a != b)                                                                                    \
    {                                                                                              \
      TEST_FAIL("%d != %d", (int)(a), (int)(b));                                                   \
    }                                                                                              \
  }
