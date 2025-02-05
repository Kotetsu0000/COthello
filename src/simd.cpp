// src/simd.cpp
#include "COthello/simd.hpp"
#include "COthello/COthello.hpp"

// NEON を使った calc_legal の実装をここに追加していく
uint64_t COthello::calc_legal_simd(const Board& board) {
    // 現状はダミー実装として、元の calc_legal を呼び出す
    return COthello::calc_legal(board);
}

// NEON を使った calc_flip の実装をここに追加していく
COthello::Flip COthello::calc_flip_simd(const Board& board, uint_fast8_t pos) {
    // 現状はダミー実装として、元の calc_flip を呼び出す
    COthello::Flip flip;
    flip.pos = 0;
    flip.flip = 0;
    return flip;
}

// NEON を使った count_player の実装をここに追加していく
int COthello::count_player_simd(uint64_t player_board) noexcept {
    // 現状はダミー実装として、元の count_player を呼び出す
    return __builtin_popcountll(player_board);
}