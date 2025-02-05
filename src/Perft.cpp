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
            Board next_board = board->copy();
            flip = next_board.calc_flip(cell);
            next_board.move_board(flip);
            res += perft(&next_board, depth - 1, false);
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
            res = perft_no_pass_count(board, depth, true); // パスが発生した次の手番は、深さを減らさない
            board->pass_turn();
            return res;
        }
        
        Flip flip;
        for (uint_fast8_t cell = 0; cell < 64; ++cell) {
            if (!((legal >> cell) & 1)) continue;
            Board next_board = board->copy();
            flip = next_board.calc_flip(cell);
            next_board.move_board(flip);
            res += perft_no_pass_count(&next_board, depth - 1, false); // パスが発生しなかった次の手番は、深さを1つ減らす
        }
        return res;
    }

} // namespace COthello