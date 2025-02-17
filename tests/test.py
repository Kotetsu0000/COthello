import random
import time

import COthello

def get_legal_list(board):
    legal_list = []
    legal = board.get_legal()
    for i in range(64):
        if (legal >> i) & 1:
            legal_list.append(i)
    return legal_list

def random_move(board):
    legal_list = board.get_legal_list()
    move = random.choice(legal_list)
    flip = board.calc_flip(move)
    board.move_board(flip)
    return board

def complete_move(board):
    move = board.complete_search()
    board.move_board(move)
    print(move.pos)
    return board

def minimize_opponent_move(board):
    move = board.minimize_opponent_move(random.randint(2, 5))
    board.move_board(move)
    print(move.pos)
    return board

all_time = 0

board = COthello.Board()

while board.count_all() < 47:
    if random.randint(0, 1):
        board = random_move(board)
    else:
        board = minimize_opponent_move(board)
board.print()
print(COthello.get_simd_name())

while not board.is_end():
    print(board.get_legal_list())
    start = time.perf_counter()
    board = complete_move(board)
    all_time += time.perf_counter() - start
    board.print()

print(board.count_player(), board.count_opponent(), all_time)

print(COthello.addv(10))
print(COthello.get_os_name())
print(COthello.get_simd_name())


