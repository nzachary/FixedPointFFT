#ifndef FPFFT_CONFIG_HPP
#define FPFFT_CONFIG_HPP

// Comment/uncomment these settings

// Default: A lookup table is used instead of calculating sine / cosine from scratch every time
// #define FPFFT_SINCOS_DONT_USE_LUT


////////////////////////////////////////////////////////////////////////
//////                                                            //////
//////                Don't touch below this point                //////
//////                                                            //////
////////////////////////////////////////////////////////////////////////

#ifndef FPFFT_SINCOS_DONT_USE_LUT
  #undef FPFFT_SINCOS_USE_LUT
  #define FPFFT_SINCOS_USE_LUT
#endif

#endif
