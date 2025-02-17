#pragma once

// board size definition
constexpr int HW = 8;
constexpr int HW_M1 = 7;
constexpr int HW_P1 = 9;
constexpr int HW2 = 64;
constexpr int HW2_M1 = 63;
constexpr int HW2_P1 = 65;

// color definition
constexpr int BLACK = 0;
constexpr int WHITE = 1;
constexpr int VACANT = 2;

// constant
constexpr int N_8BIT = 256; // 2 ^ 8
constexpr int N_16BIT = 65536; // 2 ^ 16
constexpr int INF = 100000000;
constexpr int SCORE_INF = 127;
constexpr int SCORE_MAX = 64;

inline bool is_black_like_char(char c) {
    return c == 'B' || c == 'b' || c == 'X' || c == 'x' || c == '0' || c == '*';
}

inline bool is_white_like_char(char c) {
    return c == 'W' || c == 'w' || c == 'O' || c == 'o' || c == '1';
}
