def gcd(a, b):
    if a == 0:
        return b
    return gcd(b % a, a)


def coeffs(a, b):
    if a == 0:
        return (0, 1)
    x, y = coeffs(b % a, a)
    return y - (b // a) * x, x
