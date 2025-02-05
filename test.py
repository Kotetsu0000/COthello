import COthello

board = COthello.Board()
board.reset()

# 浅い深さでテスト
print(COthello.perft(board, 0, False))
print(COthello.perft(board, 1, False))
print(COthello.perft_no_pass_count(board, 0, False))
print(COthello.perft_no_pass_count(board, 1, False))