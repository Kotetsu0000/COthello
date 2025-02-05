import COthello

board = COthello.Board()
board.reset()
board.print()

# 適当な Flip 構造体を作成 (ここでは d4 に石を置く想定)
flip = COthello.Flip()
flip.pos = 27 # d4
flip.flip = 0 # 仮の値

board.move_board(flip) # テスト用のmove_board()でflip.posに対するflipを後で計算して代入します
board.print()

board.pass_turn()
board.print()

print(f"Score: {board.score_player()}")
print(f"Is end: {board.is_end()}")
print(f"Count Player: {board.count_player()}")

board_copy = board.copy()
board_copy.print()

# 合法手の確認
legal_moves = board.get_legal()
print(f"Legal moves: {legal_moves}")

# 反転の確認 (まだ未実装なので仮の値)
flip = board.calc_flip(26)  # c4 (10進数で19)
print(f"Flipped discs for pos {flip.pos}: {flip.flip}")