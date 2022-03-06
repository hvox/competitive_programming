#!/usr/bin/env python3

from functools import cache
from math import inf
from fractions import Fraction
from collections import defaultdict


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


@cache
def number_of_combinations(k, n):
    result = Fraction(1)
    for f in range(k + 1, n + 1):
        result *= f
    for f in range(1, n - k + 1):
        result /= f
    return result


def count_sums_of_subsets(elements):
    subsets = defaultdict(int)
    subsets[0, 0] = 1
    for x in elements:
        for (sum, size), count in list(subsets.items()):
            subsets[sum + x, size + 1] += count
    return dict(subsets)


def fast_count(prices, lootbox_price, n=None):
    n = n or len(prices)
    assert min(prices) >= lootbox_price
    result = 0
    sum_prices = sum(prices)
    subsets = count_sums_of_subsets(prices)
    for (price, size), combinations in subsets.items():
        if size == len(prices):
            continue
        casino_price = (1 + Fraction(n) / (n - size)) * lootbox_price / 2
        buy_price = Fraction(sum_prices - price) / (n - size)
        price = min(buy_price, casino_price)
        probability = Fraction(combinations) / number_of_combinations(size, n)
        result += probability * price
    return result


def get_tactic(prices, x=1, n=None):
    prices = tuple(sorted(Fraction(p) for p in prices))
    n = n if n is not None else len(prices)
    return dp(prices, n, x) if min(prices) < x else fast_count(prices, x)


def execute_tactic(prices, casino_price, n=None):
    if not prices:
        print("success")
        return
    n = n or len(prices)
    price, tactic = get_tactic(prices, casino_price, n)
    print(list(map(int, prices)))
    print(f"We should use tactic: {tactic}")
    print(f"Total price: {price} or {float(price)}")
    if tactic == "buy":
        prices.pop(0)
    else:
        prices.pop(-1)
    return execute_tactic(prices, casino_price, n)


if __name__ == "__main__":
    prices = list(map(Fraction, input("Input prices of things: ").split()))
    casino_price = Fraction(input("Input price of lootbox: "))
    print(get_tactic(prices, casino_price))
