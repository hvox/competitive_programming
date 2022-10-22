# score: 16780600
import itertools
import sys


def get_score(original_matrices, matrices):
    xs, orig_products = matmult(original_matrices)
    ys, products = matmult(matrices)
    acc = sum(sum(y / x for x, y in zip(xs, ys)) for xs, ys in zip(xs, ys)) / n / n
    param = products / orig_products
    return int(max(0, 1e7 * ((acc - 1) / 2 + 1 - param) / 2)) * (acc >= 0.6)


def matmult(matrices):
    x, operations = matrices[0], 0
    for y in matrices[1:]:
        new_x = [[0] * n for _ in range(n)]
        for i in range(n):
            for j in range(n):
                for k in range(n):
                    if x[i][k] and y[k][j]:
                        new_x[i][j] += x[i][k] * y[k][j]
                        operations += 1
        x = new_x
    return x, operations


d, n = map(int, input().split())
matrices = [[list(map(int, input().split())) for _ in range(n)] for _ in range(d)]
original_matrices = [[row.copy() for row in matrix] for matrix in matrices]
for x in range(d - (n < 5)):
    for y, z in itertools.product(range(n), repeat=2):
        matrices[x][y][z] *= matrices[x][y][z] > 1
if sys.stdout.isatty():
    print(get_score(original_matrices, matrices))
else:
    for matrix in matrices:
        for row in matrix:
            print(" ".join(map(str, row)))
