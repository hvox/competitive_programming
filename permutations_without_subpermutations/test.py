from itertools import permutations
from mymath import Rational
from math import factorial


def count_subpermutationfree_permutations(n):
    def is_interval_permutation(perm):
        return max(perm) - min(perm) + 1 == len(perm)

    def is_subpermutationfree_permutation(perm):
        n = len(perm)
        for start in range(n):
            #for size in range(2, min(n - start, n - 1)):
            for end in range(start + 2, n + 1):
                if end - start == n:
                    continue
                if is_interval_permutation(perm[start : end]):
                    return False
        return True
    return sum(map(is_subpermutationfree_permutation, permutations(range(n))))


if __name__ == "__main__":
    from solution import nonsp_perms_fixed
    for n in range(10):
        print(n, "->", nonsp_perms_fixed(n), count_subpermutationfree_permutations(n))
