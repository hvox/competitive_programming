from itertools import permutations
from mymath import Rational
from math import factorial


def count_subpermutationfree_permutations(n):
    def is_interval_permutation(perm):
        return max(perm) - min(perm) + 1 == len(perm)

    def is_subpermutationfree_permutation(perm):
        n = len(perm)
        for start in range(n):
            for end in range(start + 2, n + 1):
                if end - start == n:
                    continue
                if is_interval_permutation(perm[start : end]):
                    return False
        return True
    return sum(map(is_subpermutationfree_permutation, permutations(range(n))))


if __name__ == "__main__":
    from solution import nonsp_perms
    for n in range(20):
        y1 = nonsp_perms(n)
        y2 = count_subpermutationfree_permutations(n)
        ps = factorial(n)
        print(f"{n:2}! = {ps:<11}=>{y1:>10} ~ {y2:<10}")
