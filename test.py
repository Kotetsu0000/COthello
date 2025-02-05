import COthello

board = COthello.Board()
board.reset()

# 浅い深さでテスト (例えば深さ 1-6 程度)
for depth in range(7):
    print(f"Perft({depth}) = {COthello.perft(board, depth, False)}")
    print(f"Perft_no_pass_count({depth}) = {COthello.perft_no_pass_count(board, depth, False)}")