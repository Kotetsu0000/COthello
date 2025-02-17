import time
import COthello._COthello as COthello

board = COthello.Board()
board.reset()

mode1_list = [1, 4, 12, 56, 244, 1396, 8200, 55092, 390216, 3005288, 24571284, 212258800, 1939886636, 18429641748, 184042084512, 1891832540064]
mode2_list = [1, 4, 12, 56, 244, 1396, 8200, 55092, 390216, 3005320, 24571420, 212260880, 1939899208, 18429791868, 184043158384, 1891845643044]

# 浅い深さでテスト (例えば深さ 1-6 程度)
for depth in range(13):
    start = time.perf_counter()
    perfit1 = COthello.perft(board, depth, False)
    end = time.perf_counter()
    print(f"Perft({depth:>2d})               = {perfit1}, time = {end - start:.6f}")
    assert perfit1 == mode1_list[depth], f"Perft({depth}) = {perfit1} != {mode1_list[depth]}"
    start = time.perf_counter()
    perfit2 = COthello.perft_no_pass_count(board, depth, False)
    end = time.perf_counter()
    print(f"Perft_no_pass_count({depth:>2d}) = {perfit2}, time = {end - start:.6f}")
    assert perfit2 == mode2_list[depth], f"Perft_no_pass_count({depth}) = {perfit2} != {mode2_list[depth]}"

#start = time.perf_counter()
#for i in range(10):
#    print(f'Count {i}', end='\r')
#    COthello.perft_no_pass_count(board, 10, False)
#end = time.perf_counter()
#print(f"Perft(10) 10 times = {end - start:.6f}")
