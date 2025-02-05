#include "COthello/Perft.hpp"

uint64_t COthello::perft(Board *board, int depth, bool passed) {
    if (depth == 0) {
        return 1ULL;
    }
    uint64_t res = 0;
    uint64_t legal = board->get_legal();
    if (legal == 0) {
        if (passed) {
            return 1ULL; // game over
        }
        board->pass_turn();
        res = perft(board, depth - 1, true); // pass counted as 1 move
        board->pass_turn();
        return res;
    }
    if (depth == 1) {
        return __builtin_popcountll(legal); // speedup with bulk-counting
    }
    
    // 未実装
    return res;
}

uint64_t COthello::perft_no_pass_count(Board *board, int depth, bool passed) {
    if (depth == 0) {
        return 1ULL;
    }
    uint64_t res = 0;
    uint64_t legal = board->get_legal();
    if (legal == 0) {
        if (passed) {
            return 1ULL; // game over
        }
        board->pass_turn();
        res = perft_no_pass_count(board, depth - 1, true);
        board->pass_turn();
        return res;
    }
    if (depth == 1) {
        return __builtin_popcountll(legal); // speedup with bulk-counting
    }
    // 未実装
    return res;
}