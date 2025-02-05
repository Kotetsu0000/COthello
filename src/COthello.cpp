#include <pybind11/pybind11.h>
#include "COthello/COthello.hpp"
#include "COthello/Board.hpp"
#include "COthello/Flip.hpp"
#include <iostream>
#include <string>
#include <algorithm>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

// Boardクラスのメソッドのダミー実装
COthello::Board::Board() : player(0), opponent(0) {}

void COthello::Board::reset() {
    player = 0x0000000810000000;
    opponent = 0x0000001008000000;
}

void COthello::Board::move_board(const COthello::Flip& flip) {
    player ^= (1ULL << flip.pos); // 石を置く
    player ^= flip.flip;          // ひっくり返す
    opponent ^= flip.flip;        // ひっくり返された石を取り除く
    std::swap(player, opponent);  // 手番の交代
}

void COthello::Board::undo_board(const COthello::Flip& flip) {
    std::swap(player, opponent); // 手番を戻す
    opponent ^= (1ULL << flip.pos); // 石を取り除く
    player ^= flip.flip;          // ひっくり返した石を元に戻す
    opponent ^= flip.flip;        // ひっくり返した石を元に戻す
}

void COthello::Board::pass_turn() {
    std::swap(player, opponent);
}

bool COthello::Board::is_end() const {
    if ((player | opponent) == 0xFFFFFFFFFFFFFFFFULL) {
        return true; // 盤面が全て埋まっている
    }
    // 現在のプレイヤーの合法手を取得
    uint64_t legal_moves_current = get_legal();
    if (legal_moves_current != 0) {
        return false; // 現在のプレイヤーに合法手がある
    }

    // 相手の合法手を取得
    Board temp_board = *this;
    temp_board.pass_turn();
    uint64_t legal_moves_opponent = temp_board.get_legal();

    // 両者ともに合法手がない場合、終局と判定
    return legal_moves_opponent == 0;
}

int COthello::Board::score_player() const {
    return __builtin_popcountll(player) - __builtin_popcountll(opponent);
}

int COthello::Board::count_player() const {
    return __builtin_popcountll(player);
}

COthello::Board COthello::Board::copy() const {
    Board new_board;
    new_board.player = player;
    new_board.opponent = opponent;
    return new_board;
}

std::string COthello::Board::to_str() const {
    // 仮実装: 後で更新
    return "";
}

void COthello::Board::print() const {
    for (int y = 7; y >= 0; --y) {
        for (int x = 0; x < 8; ++x) {
            int pos = y * 8 + x;
            if ((player >> pos) & 1) {
                std::cout << "X ";
            } else if ((opponent >> pos) & 1) {
                std::cout << "O ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

// 実装: 現在のボードとプレイヤーに基づいた合法手を計算
uint64_t calc_legal(const COthello::Board &board) {
    uint64_t moves, mO;
    uint64_t flip1, flip7, flip9, flip8, pre1, pre7, pre9, pre8;
    uint64_t P = board.player;
    uint64_t O = board.opponent;
    mO = O & 0x7e7e7e7e7e7e7e7eULL;
    flip1 = mO & (P << 1);         flip7  = mO & (P << 7);        flip9  = mO & (P << 9);        flip8  = O & (P << 8);
    flip1 |= mO & (flip1 << 1);    flip7 |= mO & (flip7 << 7);    flip9 |= mO & (flip9 << 9);    flip8 |= O & (flip8 << 8);
    pre1 = mO & (mO << 1);         pre7 = mO & (mO << 7);         pre9 = mO & (mO << 9);         pre8 = O & (O << 8);
    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
    flip1 |= pre1 & (flip1 << 2);  flip7 |= pre7 & (flip7 << 14); flip9 |= pre9 & (flip9 << 18); flip8 |= pre8 & (flip8 << 16);
    moves = flip1 << 1;            moves |= flip7 << 7;           moves |= flip9 << 9;           moves |= flip8 << 8;
    flip1 = mO & (P >> 1);         flip7  = mO & (P >> 7);        flip9  = mO & (P >> 9);        flip8  = O & (P >> 8);
    flip1 |= mO & (flip1 >> 1);    flip7 |= mO & (flip7 >> 7);    flip9 |= mO & (flip9 >> 9);    flip8 |= O & (flip8 >> 8);
    pre1 >>= 1;                    pre7 >>= 7;                    pre9 >>= 9;                    pre8 >>= 8;
    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
    flip1 |= pre1 & (flip1 >> 2);  flip7 |= pre7 & (flip7 >> 14); flip9 |= pre9 & (flip9 >> 18); flip8 |= pre8 & (flip8 >> 16);
    moves |= flip1 >> 1;           moves |= flip7 >> 7;           moves |= flip9 >> 9;           moves |= flip8 >> 8;
    return moves & ~(P | O);
}

// 実装: 指定された位置に石を置いた場合に反転する石を計算
COthello::Flip calc_flip(const COthello::Board &board, const int pos) {
    COthello::Flip flip;
    flip.pos = pos;
    flip.flip = 0ULL;

    uint64_t P = board.player;
    uint64_t O = board.opponent;

    // 8方向をチェック
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;

            uint64_t mask = 0ULL;
            int x = (pos % 8) + dx;
            int y = (pos / 8) + dy;

            while (x >= 0 && x < 8 && y >= 0 && y < 8) {
                int current_pos = y * 8 + x;
                uint64_t current_bit = 1ULL << current_pos;

                if (O & current_bit) {
                    mask |= current_bit;
                } else if (P & current_bit) {
                    flip.flip |= mask;
                    break;
                } else {
                    break;
                }

                x += dx;
                y += dy;
            }
        }
    }

    return flip;
}

// Boardクラスのメソッドとして実装
uint64_t COthello::Board::get_legal() const {
    return ::calc_legal(*this);
}

// Boardクラスのメソッドとして実装
COthello::Flip COthello::Board::calc_flip(uint_fast8_t pos) const {
    return ::calc_flip(*this, pos);
}

PYBIND11_MODULE(COthello, m) {
    m.doc() = "COthello: A fast Othello library using SIMD";

    m.def("add", &add, "A function that adds two numbers");

    // Boardクラスをバインド
    py::class_<COthello::Board>(m, "Board")
        .def(py::init<>())
        .def("reset", &COthello::Board::reset)
        .def("move_board", &COthello::Board::move_board)
        .def("pass_turn", &COthello::Board::pass_turn)
        .def("is_end", &COthello::Board::is_end)
        .def("score_player", &COthello::Board::score_player)
        .def("count_player", &COthello::Board::count_player)
        .def("copy", &COthello::Board::copy)
        .def("to_str", &COthello::Board::to_str)
        .def("print", &COthello::Board::print)
        .def("get_legal", &COthello::Board::get_legal)
        .def("calc_flip", &COthello::Board::calc_flip)
        .def("undo_board", &COthello::Board::undo_board);

    // Flip構造体をバインド
    py::class_<COthello::Flip>(m, "Flip")
        .def(py::init<>())
        .def_readwrite("pos", &COthello::Flip::pos)
        .def_readwrite("flip", &COthello::Flip::flip);

    m.def("perft", &COthello::perft, "Perft function");
    m.def("perft_no_pass_count", &COthello::perft_no_pass_count, "Perft function mode 2");
}