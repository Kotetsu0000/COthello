#pragma once
#include <pybind11/pybind11.h>
#include "COthello/COthello.hpp"
#include "COthello/Board.hpp"
#include "COthello/Flip.hpp"
#include <iostream>
#include <string>
#include <algorithm>
#include <omp.h>

namespace py = pybind11;

int add(int i, int j) {
    return i + j;
}

int addv(int i) {
    int res = 0;
    #pragma omp parallel for reduction(+:res)
    for (int j = 0; j < i; ++j) {
        res += j;
    }
    return res;
}

std::string get_simd_name() {
#if defined(__AVX512F)
    return "AVX512";
#elif defined(__AVX2__)
    return "AVX2";
#elif defined(__ARM_NEON)
    return "NEON";
#elif defined(__ARM_FEATURE_SVE)
    return "SVE";
#else
    return "Scalar";
#endif
}

std::string get_os_name() {
#if defined(_WIN32)
    return "Windows";
#elif defined(__linux__)
    return "Linux";
#elif defined(__APPLE__)
    return "macOS";
#else
    return "Unknown";
#endif
}



PYBIND11_MODULE(_COthello, m) {
    m.doc() = "COthello: A fast Othello library using SIMD";
    m.def("add", &add, "A function which adds two numbers");
    m.def("addv", &addv, "A function which adds two numbers");
    m.def("get_simd_name", &get_simd_name, "Get SIMD name");
    m.def("get_os_name", &get_os_name, "Get OS name");

    m.def("flip_init", &COthello::flip_init, "Flip init function");

    // Boardクラスをバインド
    py::class_<COthello::Board>(m, "Board")
        .def(py::init<>())
        .def_readwrite("player_num", &COthello::Board::player_num)
        .def("reset", &COthello::Board::reset)
        .def("move_board", &COthello::Board::move_board)
        .def("pass_turn", &COthello::Board::pass_turn)
        .def("is_end", &COthello::Board::is_end)
        .def("score_player", &COthello::Board::score_player)
        .def("count_player", &COthello::Board::count_player)
        .def("count_opponent", &COthello::Board::count_opponent)
        .def("count_all", &COthello::Board::count_all)
        .def("copy", &COthello::Board::copy)
        //.def("to_str", &COthello::Board::to_str)
        .def("print", &COthello::Board::print)
        .def("get_legal", &COthello::Board::get_legal)
        .def("calc_flip", &COthello::Board::calc_flip)
        .def("undo_board", &COthello::Board::undo_board)
        .def("get_legal_list", &COthello::Board::get_legal_list)
        .def("complete_search", &COthello::Board::complete_search, py::arg("debug") = false)
        .def("minimize_opponent_move", &COthello::Board::minimize_opponent_move, py::arg("depth"), py::arg("debug") = false)
        .def("simulate_search_move", &COthello::Board::simulate_search_move, py::arg("depth"), py::arg("simulation_count") = 1000, py::arg("debug") = false);

    // Flip構造体をバインド
    py::class_<COthello::Flip>(m, "Flip")
    //    .def(py::init<>())
        .def_readwrite("pos", &COthello::Flip::pos);
    //    .def_readwrite("flip", &COthello::Flip::flip);

    m.def("perft", &COthello::perft, "Perft function");
    m.def("perft_no_pass_count", &COthello::perft_no_pass_count, "Perft function mode 2");
}