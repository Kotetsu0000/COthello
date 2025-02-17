#pragma once

#include <pybind11/pybind11.h>
#include <vector>
#include <omp.h>

#include "COthello/Flip.hpp"
#include "COthello/Board.hpp"
#include "COthello/common.hpp"

namespace py = pybind11;

#define BLACK 0
#define WHITE 1

namespace COthello {
    // コンストラクタ
    Board::Board() {
        reset();
    }

    /*Board::Board(uint64_t player_, uint64_t opponent_)
            : player(player_), opponent(opponent_) {}
        
    Board::Board(std::string board_str) {
        from_str(board_str);
    }*/

    // 盤面をリセット
    void Board::reset() {
        player = 0x0000000810000000;
        opponent = 0x0000001008000000;
        player_num = 0;
    }

    // 石を置く
    void Board::move_board(const Flip &flip) {
        if (flip.pos == 64) {
            pass_turn();
            return;
        }
        player ^= (1ULL << flip.pos); // 石を置く
        player ^= flip.flip;          // ひっくり返す
        opponent ^= flip.flip;        // ひっくり返された石を取り除く
        std::swap(player, opponent);  // 手番の交代
        player_num =  1 - player_num;
    }

    // 手番を戻す
    void Board::undo_board(const Flip &flip) {
        std::swap(player, opponent); // 手番を戻す
        opponent ^= (1ULL << flip.pos); // 石を取り除く
        player ^= flip.flip;          // ひっくり返した石を元に戻す
        opponent ^= flip.flip;        // ひっくり返した石を元に戻す
    }

    // 手番をパス
    void Board::pass_turn() {
        std::swap(player, opponent);
        player_num = 1 - player_num;
    }

    // ゲーム終了判定
    bool Board::is_end() const {
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

    // プレイヤーのスコアを計算
    int Board::score_player() const {
        return __builtin_popcountll(player) - __builtin_popcountll(opponent);
    }

    // プレイヤーの石の数をカウント
    int Board::count_player() const {
        return __builtin_popcountll(player);
    }

    // 相手の石の数をカウント
    int Board::count_opponent() const {
        return __builtin_popcountll(opponent);
    }

    // 盤面の石の数をカウント
    int Board::count_all() const {
        return __builtin_popcountll(player | opponent);
    }

    // 盤面をコピー
    Board Board::copy() const {
        Board new_board;
        new_board.player = player;
        new_board.opponent = opponent;
        new_board.player_num = player_num;
        return new_board;
    }

    void Board::print() const {
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

    // 合法手を取得(Not SIMD)
    uint64_t calc_legal(const Board &board) {
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

    // 石を置いたときにひっくり返る石を計算(Not SIMD)
    Flip calc_flip(const Board &board, const uint_fast8_t pos) {
        Flip flip;
        flip.pos = pos;
        flip.flip = 0ULL;

        flip.calc_flip(board.player, board.opponent, pos);

        return flip;
    }

    // 合法手を取得(継承)
    uint64_t Board::get_legal() const {
        return COthello::calc_legal(*this);
    }

    // 石を置いたときにひっくり返る石を計算(継承)
    Flip Board::calc_flip(uint_fast8_t pos) const {
        return COthello::calc_flip(*this, pos);
    }

    // 合法手のPythonリスト返却
    py::list Board::get_legal_list(){
        py::list legal_list;
        uint64_t legal = this->get_legal();
        for (int i = 0; i < 64; ++i) {
            if ((legal >> i) & 1) {
                legal_list.append(i);
            }
        }
        return legal_list;
    }

    // αβ法（ネガマックス形式）による探索で、最後まで全探索し、最善手のスコアを返す
    int Board::complete_score(int alpha, int beta) {
        if (this->is_end()){
            return this->score_player();
        }
        uint64_t legal = this->get_legal();
        if (legal == 0) {
            Board next_board = this->copy();
            next_board.pass_turn();
            return -next_board.complete_score(-beta, -alpha);
        }
        
        // 合法手をベクターに格納する
        std::vector<uint_fast8_t> moves;
        for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)) {
            moves.push_back(cell);
        }
        
        // ムーブオーダリング用の静的重み付けテーブル
        // ここでは、コーナーは高評価、隣接マスは低評価とする例です
        static const int move_weights[64] = {
             100, -20,  10,   5,   5,  10, -20, 100,
             -20, -50,  -2,  -2,  -2,  -2, -50, -20,
              10,  -2,  -1,  -1,  -1,  -1,  -2,  10,
               5,  -2,  -1,  -1,  -1,  -1,  -2,   5,
               5,  -2,  -1,  -1,  -1,  -1,  -2,   5,
              10,  -2,  -1,  -1,  -1,  -1,  -2,  10,
             -20, -50,  -2,  -2,  -2,  -2, -50, -20,
             100, -20,  10,   5,   5,  10, -20, 100
        };

        // 重み付けに従い、評価値が高い手を先に探索するようにソート
        std::sort(moves.begin(), moves.end(), [&](uint_fast8_t a, uint_fast8_t b) {
            return move_weights[a] > move_weights[b];
        });
        
        int score;
        // 並び替えた候補手で再帰的に探索
        for (uint_fast8_t cell : moves) {
            Board next_board = this->copy();
            Flip flip = next_board.calc_flip(cell);
            next_board.move_board(flip);
            score = -next_board.complete_score(-beta, -alpha);
            if (score > alpha) {
                alpha = score;
            }
            if (alpha >= beta) { // βカットオフ
                break;
            }
        }
        return alpha;
    }
    
    // αβ法（ネガマックス形式）を用いて、最後まで全探索し、最善手を返す
    Flip Board::complete_search(bool debug) {
        uint64_t legal = this->get_legal();
        Flip global_best_flip;
        if (this->is_end() || legal == 0) {
            global_best_flip.pos = 0;
            global_best_flip.flip = 0;
            return global_best_flip;
        }

        // 合法手の候補をベクターに格納する
        std::vector<uint_fast8_t> moves;
        for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)) {
            moves.push_back(cell);
        }

        int global_best_score = -64;
        global_best_flip.pos = 0;
        global_best_flip.flip = 0;

        // 各候補手は独立に評価可能なので、並列化する
        #pragma omp parallel
        {
            int local_best_score = -64;
            Flip local_best_flip;
            #pragma omp for nowait
            for (int i = 0; i < moves.size(); i++) {
                uint_fast8_t cell = moves[i];
                Board next_board = this->copy();
                Flip flip = next_board.calc_flip(cell);
                next_board.move_board(flip);
                int score = -next_board.complete_score(-64, 64);
                if (debug) {
                    #pragma omp critical
                    {
                        std::cout << "pos: " << (int)cell << " score: " << score << std::endl;
                    }
                }
                if (score > local_best_score) {
                    local_best_score = score;
                    local_best_flip = flip;
                }
            }
            #pragma omp critical
            {
                if (local_best_score > global_best_score) {
                    global_best_score = local_best_score;
                    global_best_flip = local_best_flip;
                }
            }
        }

        return global_best_flip;
    }

    // Board::minimize_opponent_score() メソッド
    // 指定した深さまでアルファβ法で探索し、評価値（＝-合法手の数）を返します。
    int Board::minimize_opponent_score(int depth, int alpha, int beta) {
        // 探索深さが0の場合、現在の手番の合法手数（＝相手の置ける手数）を評価値とする。
        // ※ 現在のBoardは、実際には手を打った直後に手番が交代しているので、
        //   この評価値は「相手の選択肢の多さ」を表し、少なければ評価が高くなります。
        if (depth == 0) {
            int moves = __builtin_popcountll(this->get_legal());
            return moves;
        }

        uint64_t legal = this->get_legal();
        // もし合法手が存在しないなら、パスして探索を継続
        if (legal == 0) {
            Board next_board = this->copy();
            next_board.pass_turn();
            return -next_board.minimize_opponent_score(depth - 1, -beta, -alpha);
        }
        
        int score;
        // 各合法手について探索（ネガマックス形式）
        // first_bit/next_bit は既存のビット操作ヘルパー関数を利用しています
        for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)) {
            Board next_board = this->copy();
            Flip flip = next_board.calc_flip(cell);
            next_board.move_board(flip);
            score = -next_board.minimize_opponent_score(depth - 1, -beta, -alpha);
            if (score > alpha) {
                alpha = score;
            }
            if (alpha >= beta) { // βカットオフ
                break;
            }
        }
        return alpha;
    }

    // Board::minimize_opponent_move() メソッド
    // 指定した探索深さにおいて、相手の合法手数（置ける手の数）が少なくなる手（Flip）を返します。
    Flip Board::minimize_opponent_move(int depth, bool debug=false) {
        uint64_t legal = this->get_legal();
        Flip best_flip;
        if (this->is_end() || legal == 0) {
            best_flip.pos = 0;
            best_flip.flip = 0;
            return best_flip;
        }

        int alpha = -64;
        int beta = 64;
        int best_score = -64;
        int score;
        // 各候補手について評価
        for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)) {
            Board next_board = this->copy();
            Flip flip = next_board.calc_flip(cell);
            next_board.move_board(flip);
            score = -next_board.minimize_opponent_score(depth - 1, -beta, -alpha);
            if (debug){
                std::cout << "pos: " << (int)cell << " score: " << score << std::endl;
            }
            if (score > best_score) {
                best_score = score;
                best_flip = flip;
            }
            if (score > alpha) {
                alpha = score;
            }
            if (alpha >= beta) { // ルートでのカットオフ
                break;
            }
        }
        return best_flip;
    }

    // 現在の盤面から終局まで指定の回数シミュレートして、平均スコアを返す
    double Board::simulate_score(int count) {
        double sum = 0;
        #pragma omp parallel for reduction(+:sum) schedule(dynamic)
        for (int i = 0; i < count; ++i) {
            Board temp_board = this->copy();
            while (!temp_board.is_end()) {
                uint64_t legal = temp_board.get_legal();
                if (legal == 0) {
                    temp_board.pass_turn();
                    continue;
                }
                uint_fast8_t cell = first_bit(&legal);
                Flip flip = temp_board.calc_flip(cell);
                temp_board.move_board(flip);
            }
            sum += temp_board.score_player();
        }
        return sum / count;
    }

    // 指定した探索深さにおいて、シミュレーションを行い、最善手のスコアを返す
    double Board::simulate_search_score(int depth, int simulation_count, double alpha, double beta) {
        uint64_t legal = get_legal();
        // 合法手がない場合はパスして探索を継続
        if (legal == 0) {
            Board temp_board = this->copy();
            temp_board.pass_turn();
            return -temp_board.simulate_search_score(depth, simulation_count, -beta, -alpha);
        }
        // 探索深さが0になったら、simulate関数で評価する
        if (depth == 0) {
            return simulate_score(simulation_count);
        }
        // 各合法手について、アルファベータ法を導入してシミュレーション探索を行う
        for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)) {
            Board next_board = this->copy();
            Flip flip = next_board.calc_flip(cell);
            next_board.move_board(flip);
            double score = -next_board.simulate_search_score(depth - 1, simulation_count, -beta, -alpha);
            if (score > alpha) {
                alpha = score;
            }
            if (alpha >= beta) { // βカット: これ以上の探索は不要
                break;
            }
        }
        return alpha;
    }

    // 指定した探索深さにおいて、シミュレーションを行い、最善手を返す
    Flip Board::simulate_search_move(int depth, int simulation_count, bool debug) {
        uint64_t legal = get_legal();
        if (legal == 0) {
            // 合法手が存在しない場合は、パスを示す無効なFlipを返す
            Flip res;
            res.pos = 64;  // 無効な位置として設定
            res.flip = 0;
            return res;
        }
        if (depth == 0) {
            // 探索深さが0の場合は、シミュレーションを行わず最初の合法手を返す
            uint_fast8_t cell = first_bit(&legal);
            return calc_flip(cell);
        }
        
        double best_score = -std::numeric_limits<double>::infinity();
        Flip best_flip = {0, 0};
        
        for (uint_fast8_t cell = first_bit(&legal); legal; cell = next_bit(&legal)) {
            Board next_board = this->copy();
            Flip current_flip = next_board.calc_flip(cell);
            next_board.move_board(current_flip);
            
            double score;
            if (depth - 1 == 0) {
                score = -next_board.simulate_score(simulation_count);
            } else {
                score = -next_board.simulate_search_score(depth - 1, simulation_count,
                                                    -std::numeric_limits<double>::infinity(),
                                                    std::numeric_limits<double>::infinity());
            }
            
            if (debug) {
                //std::cout << "simulate_search_move: cell: " << static_cast<int>(cell) << " score: " << score << std::endl;
            }
            if (score > best_score) {
                best_score = score;
                best_flip = current_flip;
            }
        }
        if (debug) {
            std::cout << "simulate_search_move: best_score: " << best_score << std::endl;
        }
        return best_flip;
    }

    // 盤面を文字列に変換
    /*std::string Board::to_str() const {
        std::string res;
        uint64_t cell_bit = 1ULL << (HW2 - 1);
        for (int i = 0; i < HW2; ++i) {
            if (player & cell_bit) {
                res += "X";
            } else if (opponent & cell_bit) {
                res += "O";
            } else {
                res += "-";
            }
            cell_bit >>= 1;
        }
        res += " X";
        return res;
    }

    // 文字列から盤面を復元
    bool Board::from_str(std::string board_str) { // returns OK: true NG: false
        board_str.erase(std::remove_if(board_str.begin(), board_str.end(), ::isspace), board_str.end());
        if (board_str.length() != HW2 + 1) {
            std::cerr << "[ERROR] invalid argument got length " << board_str.length() << " expected " << HW2 + 1 << std::endl;
            return false;
        }
        player = 0ULL;
        opponent = 0ULL;
        for (int i = 0; i < HW2; ++i) {
            if (is_black_like_char(board_str[i])) {
                player |= 1ULL << (HW2_M1 - i);
            } else if (is_white_like_char(board_str[i])) {
                opponent |= 1ULL << (HW2_M1 - i);
            }
        }
        if (is_white_like_char(board_str[HW2])) {
            std::swap(player, opponent);
        } else if (!is_black_like_char(board_str[HW2])) {
            std::cerr << "[ERROR] invalid player argument" << std::endl;
            return false;
        }
        return true;
    }*/

}// namespace COthello
