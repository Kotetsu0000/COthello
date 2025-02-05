#pragma once

#include "COthello/Board.hpp"

namespace COthello {

    uint64_t perft(Board *board, int depth, bool passed);
    uint64_t perft_no_pass_count(Board *board, int depth, bool passed);

}