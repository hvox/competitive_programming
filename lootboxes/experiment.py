from solution import get_tactic

# there is a theory:
# if the price of a lootbox is low enough, than the optimal tactic is
# to start by buying lootboxes and proceed by buying things.
# this code confirms it's true


def buy_things(prices, casino_price, n):
    if not prices:
        return "success"
    price, tactic = get_tactic(prices, casino_price, n)
    if tactic == "buy":
        return buy_things(prices[1:], casino_price, n)
    else:
        return "fail"


def buy_lootboxes(prices, casino_price, n):
    if not prices:
        return "success"
    price, tactic = get_tactic(prices, casino_price, n)
    if tactic == "buy":
        return buy_things(prices[1:], casino_price, n)
    else:
        return buy_lootboxes(prices[:-1], casino_price, n)


import random

for s in range(2, 10):
    for i in range(5):
        casino_price = random.randint(1, 1000)
        prices = [random.randint(casino_price, 1000) for _ in range(s)]
        print(s, "->", buy_lootboxes(prices, casino_price, len(prices)))
