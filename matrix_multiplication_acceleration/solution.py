from random import randint
d, n = map(int, input().split())
matrices = [[list(map(int, input().split())) for _ in range(n)] for _ in range(d)]
for _ in range(n):
    matrices[randint(0, d - 1)][randint(0, n - 1)][randint(0, n - 1)] = 0
for matrix in matrices:
    for row in matrix:
        print(" ".join(map(str, row)))
