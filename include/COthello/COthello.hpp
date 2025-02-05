// include/COthello/COthello.hpp
#pragma once

#include "COthello/Board.hpp"
#include "COthello/Flip.hpp"
#include "COthello/Perft.hpp"
#include "COthello/simd.hpp"

// COthello ライブラリ全体で共通して使う型定義などをここに追加していく (現時点では空)
namespace COthello {
    // 例: 共通の定数
    // constexpr int BOARD_SIZE = 8;

    // 例: 共通の型定義
    // using board_t = uint64_t;

    // calc_legal と calc_flip は Board クラスのメソッドではないため、
    // COthello 名前空間内で前方宣言
    uint64_t calc_legal(const Board& board);
    Flip calc_flip(const Board& board, uint_fast8_t pos);
}