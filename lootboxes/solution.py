from functools import cache
from math import inf
from fractions import Fraction


@cache
def dp(prices, n, x):
    if len(prices) == 0:
        return (0, "nothing")

    best_tactic = (inf, None)
    min_price = min(prices)

    # tactic: BUY MINIMAL
    for i in range(len(prices)):
        if prices[i] == min_price:
            prices1 = prices[:i] + prices[i + 1 :]
            break
    prices1 = (min_price + dp(prices1, n, x)[0], "buy")
    best_tactic = min(best_tactic, prices1)

    # tactic: USE CASINO UNTIL WIN
    avg_price = 0
    casino_price = (1 + Fraction(n) / len(prices)) * x / 2
    for i in range(len(prices)):
        prices2 = prices[:i] + prices[i + 1 :]
        avg_price += (dp(prices2, n, x)[0] + casino_price) / len(prices)
    best_tactic = min(best_tactic, (avg_price, "casino"))
    return best_tactic


def get_tactic(prices, x=1, n=None):
    prices = tuple(sorted(Fraction(p) for p in prices))
    n = n if n is not None else len(prices)
    return dp(prices, n, Fraction(x))


prices = list(map(Fraction, input("Input prices of things: ").split()))
casino_price = Fraction(input("Input price of lootbox: "))
price, tactic = get_tactic(prices, casino_price)
print(f"We should use tactic: {tactic}")
print(f"Total price: {price} or {float(price)}")
