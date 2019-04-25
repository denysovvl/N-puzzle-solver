#!/usr/bin/python
import numpy as np
import heapq as pq
import random
import sys

def get_new(board, di):
    if di == 0:
        idx = np.argwhere(board==0)
        idx = idx[0]
        if idx[1] < board.shape[1] - 1:
            board[idx[0]][idx[1]] = board[idx[0]][idx[1] + 1]
            board[idx[0]][idx[1] + 1] = 0
    elif di == 1:
        idx = np.argwhere(board==0)
        idx = idx[0]
        if idx[1] > 0:
            board[idx[0]][idx[1]] = board[idx[0]][idx[1] - 1]
            board[idx[0]][idx[1] - 1] = 0
    elif di == 2:
        idx = np.argwhere(board==0)
        idx = idx[0]
        if idx[0] < board.shape[0] - 1:
            board[idx[0]][idx[1]] = board[idx[0] + 1][idx[1]]
            board[idx[0] + 1][idx[1]] = 0
    elif di == 3:
        idx = np.argwhere(board==0)
        idx = idx[0]
        if idx[0] > 0:
            board[idx[0]][idx[1]] = board[idx[0] - 1][idx[1]]
            board[idx[0] - 1][idx[1]] = 0
    return board

board = np.array(
    [
    [1,2,3,4],
    [5,6,7, 8],
    [9,10,11,12],
    [13,14,15,0]
    ])


random.seed(3)
for i in range(int(sys.argv[1])):
    a = random.randint(0, 4)
    board = get_new(board, a)

print ('{')
for i in board:
    print ('{%d, %d, %d, %d},' % (i[0],  i[1], i[2], i[3]))
print ('}')

