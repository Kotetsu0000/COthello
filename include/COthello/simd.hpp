// include/COthello/simd.hpp
#pragma once

#include <cstdint>
#ifdef __ARM_NEON
#include <arm_neon.h> // NEON intrinsics
#endif
#include "COthello/Board.hpp"
#include "COthello/Flip.hpp"

namespace COthello {

// SIMD関連の関数や定数をここに宣言する
    uint64_t calc_legal_simd(const Board& board); // ここで宣言
    Flip calc_flip_simd(const Board& board, uint_fast8_t pos);
    int count_player_simd(uint64_t player_board) noexcept;

}