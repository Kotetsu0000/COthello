import COthello

board = COthello.Board()
board.reset()

# SIMD版の関数呼び出し (現在はダミー実装なので、非SIMD版と同じ結果になるはず)
legal_moves_simd = COthello.calc_legal_simd(board)
print(f"Legal moves (SIMD): {legal_moves_simd}")

flip_simd = COthello.calc_flip_simd(board, 27)  # d4
print(f"Flipped discs (SIMD): pos={flip_simd.pos}, flip={flip_simd.flip}")

count_simd = COthello.count_player_simd(board.player)
print(f"Player's disc count (SIMD): {count_simd}")