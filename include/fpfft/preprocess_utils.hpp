#ifndef FPFFT_PREPROCESS_UTILS_HPP
#define FPFFT_PREPROCESS_UTILS_HPP

// Preprocessor utilities and macros

#define _FPFFT_QUOTE(v) #v
#define _FPFFT_STRINGIFY(v) _FPFFT_QUOTE(v)

#define _FPFFT_EXPAND(x) x
#define _FPFFT_GET_MACRO_ARGS_1(ARG1, name, ...) name
#define _FPFFT_GET_MACRO_ARGS_2(ARG1, ARG2, name, ...) name
#define _FPFFT_GET_MACRO_ARGS_3(ARG1, ARG2, ARG3, name, ...) name
#define _FPFFT_GET_MACRO_ARGS_4(ARG1, ARG2, ARG3, ARG4, name, ...) name

#endif
