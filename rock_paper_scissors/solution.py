from itertools import product
def cmp(A, B):
    A_wins, B_wins = 0, 0
    for i, j in product(tuple(range(4)), repeat=2):
        A_wins += int(A[i] > B[j])
        B_wins += int(A[i] < B[j])
    return A_wins - B_wins

for _ in range(int(input())):
    facets = tuple(map(int, input().split()))
    A, B = facets[:4], facets[4:]
    cmp_AB = cmp(A, B)
    if cmp_AB == 0:
        print("no")
        continue
    if cmp_AB < 0:
        A, B = B, A
    for C in product(tuple(range(1, 11)), repeat=4):
        if cmp(B, C) > 0 and cmp(C, A) > 0:
            print("yes")
            break
    else:
        print("no")
