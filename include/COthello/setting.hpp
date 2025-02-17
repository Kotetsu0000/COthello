#pragma once
#include <string>

#define IS_GGS_TOURNAMENT false

#if defined(__AVX512BW__) && defined(__AVX512DQ__) && defined(__AVX512VL__) && defined(__AVX512F__)
    #define USE_SIMD true
    #define HAS_AVX512
#elif defined(__AVX2__)
    #define USE_SIMD true
#elif defined(__ARM_NEON) || defined(__ARM_NEON__)
    #define USE_SIMD true
    #define USE_ARM true
    #define USE_NEON true // ARM NEON を使うことを明示
#endif
