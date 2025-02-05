#pragma once

#include <cstdint>
#include <string>
#include <iostream>
#include "COthello/Flip.hpp"

namespace COthello {

class Board {
public:
    uint64_t player;
    uint64_t opponent;

    Board();
    void reset();
    void move_board(const Flip& flip);
    void pass_turn();
    bool is_end() const;
    int score_player() const;
    int count_player() const;
    Board copy() const;
    std::string to_str() const;
    void print() const;
    void undo_board(const Flip& flip); // 追加

    uint64_t get_legal() const;
    Flip calc_flip(uint_fast8_t pos) const;
};

}