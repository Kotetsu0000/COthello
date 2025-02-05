#include "COthello/Perft.hpp"

namespace COthello { // COthello 名前空間で囲む

    uint64_t perft(Board *board, int depth, bool passed) {
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
        
        Flip flip;
        for (uint_fast8_t cell = 0; cell < 64; ++cell) {
            if (!((legal >> cell) & 1)) continue;
            flip = board->calc_flip(cell);
            board->move_board(flip);
            res += perft(board, depth - 1, false);
            board->undo_board(flip);
        }
        return res;
    }

    uint64_t perft_no_pass_count(Board *board, int depth, bool passed) {
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

        Flip flip;
        for (uint_fast8_t cell = 0; cell < 64; ++cell) {
            if (!((legal >> cell) & 1)) continue;
            flip = board->calc_flip(cell);
            board->move_board(flip);
            res += perft_no_pass_count(board, depth - 1, false);
            board->undo_board(flip);
        }
        return res;
    }

} // namespace COthello