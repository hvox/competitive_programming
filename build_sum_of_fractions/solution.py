from math import gcd
from mymath import coeffs

# solve p * x + q * y = z, such that 0 < y < p and 0 < x < q


def solve(p, q, z):
    g = gcd(p, q)
    if z % g == 0:
        x, y = (z // g * x for x in coeffs(p, q))
        dx, dy = q // g, p // g
        x, y = x % dx, y + (x // dx) * dy
        if 0 < x < q and 0 < y < p:
            return x, y


def factors(n):
    for p in range(2, int(n**0.5) + 1):
        if n % p == 0:
            yield p, n // p


def build_from_fractions(n):
    for p, q in factors(n):
        if solution := solve(q, p, n - 1):
            return solution, (p, q)


n = int(input("Input number to build: "))
if solution := build_from_fractions(n):
    (x, y), (p, q) = solution
    print(f"{x}/{p} + {y}/{q} + 1/{n} = 1")
else:
    print(f"failed to build {n}.")
