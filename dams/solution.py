from itertools import chain, combinations, product


def build_dam_system(required, allowability):
    if required[0] != (1, 1, 1):
        return [(0, 0, 0)] * len(required), [1] * len(required), []
    targets = {r: set() for r in product(range(2), repeat=3)}
    for town, r in list(enumerate(required))[1:]:
        targets[r].add(town)
    sources = {(1, 1, 1): 0}
    towns = [(1, 1, 1)] + [(0, 0, 0)] * (len(required) - 1)
    dams, streams = [1] * len(required), []
    done = False
    while not done:
        for ss in chain(combinations(sources, 1), combinations(sources, 2)):
            t = (0, 0, 0)
            for s in ss:
                t = tuple(max(x, y) for x, y in zip(t, s))
            ss = {sources[s] for s in ss}
            if not targets[t]:
                continue
            trgt = next(iter(targets[t]))
            for s in ss:
                streams.append((s, trgt))
            targets[t].remove(trgt)
            towns[trgt] = t
            next_t = tuple(x * y for x, y in zip(t, allowability[trgt]))
            if next_t in sources:
                dams[trgt] = 0
                next_t = t
            sources[next_t] = trgt
            break
        else:
            done = True
    return towns, dams, streams


if __name__ == "__main__":
    n = int(input("Number of towns: "))
    print("Input required water types for every town.")
    required = []
    for _ in range(n):
        required.append(tuple(map(lambda x: x == "1", input().split())))
    print("For every town input water types that can go through dams.")
    allowability = []
    for _ in range(n):
        allowability.append(tuple(map(lambda x: x == "1", input().split())))
    towns, dams, streams = build_dam_system(required, allowability)
    if towns == required:
        print("Ok")
        print("Dambs:", " ".join(map(str, dams)))
        print("Streams: ")
        for x, y in streams:
            print(x + 1, "->", y + 1)
    else:
        print("It's impossible to build such a system")
