#pragma once
#include <pybind11/pybind11.h>
#include <cstdint>
#include <string>
#include <iostream>

#include "COthello/bit.hpp"
#include "COthello/Flip.hpp"

namespace py = pybind11;

namespace COthello {

    class Board {
        public:
            uint64_t player;
            uint64_t opponent;
            int8_t player_num;

            Board();
            /*Board(uint64_t player, uint64_t opponent);
            Board(std::string board_str);*/

            void reset();
            void move_board(const Flip &flip);
            void undo_board(const Flip &flip);
            void pass_turn();
            bool is_end() const;
            int score_player() const;
            int count_player() const;
            int count_opponent() const;
            int count_all() const;
            Board copy() const;
            void print() const;
            uint64_t get_legal() const;
            Flip calc_flip(uint_fast8_t pos) const;
            py::list get_legal_list();
            int complete_score(int alpha, int beta);
            Flip complete_search(bool debug);
            int minimize_opponent_score(int depth, int alpha, int beta);
            Flip minimize_opponent_move(int depth, bool debug);
            double simulate_score(int count);
            double simulate_search_score(int depth, int simulation_count, double alpha, double beta);
            Flip simulate_search_move(int depth, int simulation_count, bool debug);


            //std::string to_str() const;
            //bool from_str(std::string board_str);
    };
    uint64_t calc_legal(const Board& board);
    Flip calc_flip(const Board& board, uint_fast8_t pos);
}