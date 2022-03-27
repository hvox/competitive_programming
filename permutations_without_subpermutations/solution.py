from functools import cache
from math import factorial


@cache
# count permutations that have permutations it their prefixes
def prefixed(n):
    result = sum(factorial(n - k) * nonprefixed(k) for k in range(1, n))
    # print("prefixed", n, "->", result)
    return result


@cache
# count permutations that don't have permutations it their prefixes
def nonprefixed(n):
    result = factorial(n) - prefixed(n)
    # print("nonprefixed", n, "->", result)
    return result


@cache
# number of ways to choose K continious intervals of N elements
def intervals(k, n):
    if k == n:
        return 1
    result = sum(
        intervals(k - 1, n - s) * factorial(s) for s in range(1, n + 1)
    )
    # print("intervals", n, "->", result)
    return result


@cache
# number of permutations without subpermutations
def nonsp_perms(n):
    return factorial(n) - sum(
        intervals(k, n) * nonsp_perms(k) for k in range(2, n)
    )
    # number of permutations with exactly two subpermutations
    s2 = sum(factorial(n - k) * nonprefixed(k) for k in range(1, n)) * 2
    # number of permutations with many subpermutations
    s3 = sum(intervals(k, n) * nonsp_perms(k) for k in range(3, n))
    # print("nonsp_perms", n, "->", factorial(n) - s2 - s3)
    return factorial(n) - s2 - s3


def nonsp_perms_fixed(n):
    y = nonsp_perms(n)
    if n >= 3:
        if n % 2:
            y += 2
        else:
            y -= 2
    return y
