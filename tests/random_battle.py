import random
import time

import COthello

def random_move(board):
    legal_list = board.get_legal_list()
    move = random.choice(legal_list)
    flip = board.calc_flip(move)
    board.move_board(flip)
    return board

def complete_move(board):
    move = board.complete_search()
    board.move_board(move)
    #print(move.pos)
    return board

def minimize_opponent_move(board):
    move = board.minimize_opponent_move(5)
    #print(move.pos in board.get_legal_list(), move.pos, board.get_legal_list())
    board.move_board(move)
    return board

def simulate_search_move(board):
    depth = 1#ßrandom.randint(2, 5)
    #print(f'simulate_search_move start depth: {depth}')
    move = board.simulate_search_move(depth)#, debug=True
    #print(move.pos in board.get_legal_list(), move.pos, board.get_legal_list())
    board.move_board(move)
    #print(board.count_player(), board.count_opponent())
    return board

def battle(random_player=0, text=''):
    board = COthello.Board()

    while not board.is_end():
        if board.get_legal() == 0:
            print(f'パス, {text}')
            board.pass_turn()
            continue
        if board.player_num == random_player:
            board = random_move(board)
        else:
            if board.count_all() < 48:
                #board = minimize_opponent_move(board)
                #board = simulate_search_move(board)
                board = random_move(board)
            else:
                board = complete_move(board)

    print(f'ゲーム終了. 結果: {board.count_player()} - {board.count_opponent()}, {text}')
    if board.player_num == random_player:
        random_player_stone = board.count_player()
        cpu_stone = board.count_opponent()
    else:
        random_player_stone = board.count_opponent()
        cpu_stone = board.count_player()

    if random_player_stone > cpu_stone:
        print(f'勝者: ランダムプレイヤー, Random: {random_player_stone}, CPU: {cpu_stone}, {text}')
        return 0
    elif random_player_stone < cpu_stone:
        print(f'勝者: CPU, Random: {random_player_stone}, CPU: {cpu_stone}, {text}')
        return 1
    else:
        print(f'引き分け, Random: {random_player_stone}, CPU: {cpu_stone}, {text}')
        return 2

def random_battle():
    board = COthello.Board()
    while not board.is_end():
        if board.get_legal() == 0:
            print('パス')
            board.pass_turn()
            continue
        board = random_move(board)

    if board.player_num == 0:
        black = board.count_player()
        white = board.count_opponent()
    else:
        black = board.count_opponent()
        white = board.count_player()

    if black > white:
        print(f'勝者: 黒, 黒: {black}, 白: {white}')
        return 0
    elif black < white:
        print(f'勝者: 白, 黒: {black}, 白: {white}')
        return 1
    else:
        print(f'引き分け, 黒: {black}, 白: {white}')
        return 2

if __name__ == '__main__':
    win_count = [0, 0, 0]

    for i in range(1000):
        win_count[battle(0)] += 1
        print(win_count, f'試合数: {i + 1}, 現在の勝率: {win_count[1] / sum(win_count)*100:.2f}%')
        win_count[battle(1)] += 1
        print(win_count, f'試合数: {i + 1}, 現在の勝率: {win_count[1] / sum(win_count)*100:.2f}%')
        #win_count[random_battle()] += 1
        #print(win_count, f'試合数: {i + 1}, 現在の勝率: {win_count[1] / (i + 1)*100:.2f}%')
