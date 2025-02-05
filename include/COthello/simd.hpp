// include/COthello/simd.hpp
#pragma once

#include <cstdint>

// xsimd をインクルード
#include "xsimd/xsimd.hpp"

#include "COthello/Board.hpp"
#include "COthello/Flip.hpp"

namespace COthello {

// SIMD レジスタ型を定義 (xsimd を使用)
#if defined(__ARM_NEON) || defined(_M_ARM) || defined(_M_ARM64)
      using simd_register_t = xsimd::batch<uint64_t, xsimd::neon>; // NEONの場合
#elif defined(__AVX2__)
      using simd_register_t = xsimd::batch<uint64_t, xsimd::avx2>; // AVX2の場合
#else
    using simd_register_t = uint64_t; // SIMD非対応の場合は、uint64_tを使用
#endif

    uint64_t calc_legal_simd(const Board& board);
    Flip calc_flip_simd(const Board& board, uint_fast8_t pos);
    int count_player_simd(uint64_t player_board) noexcept;

}