#include "COthello/simd.hpp"

// NEON を使った calc_legal の実装をここに追加していく
uint64_t COthello::calc_legal_simd(const Board& board) {
    // 現状はダミー実装として、元の calc_legal を呼び出す
    return calc_legal(board);
}