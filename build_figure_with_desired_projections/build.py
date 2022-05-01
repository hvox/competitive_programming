def build_toy(X, Y, Z):
    if X > Y: return [(x, y, z) for y, x, z in build_toy(Y, X, Z)]
    if Y > Z: return [(x, y, z) for x, z, y in build_toy(X, Z, Y)]
    if X * Y < Z: return None
    result = set()
    for y in range(0, X):
        result.add((y, y, 0))
    for x in range(X, Y):
        result.add((x, y, 0))
    for x in range(0, Y):
        for y in range(0, X):
            if len(result) == Z:
                break
            result.add((x, y, 0))
    return result

z, y, x = map(int, input().split())
toy = build_toy(x, y, z)
if toy:
    print(len(toy))
    for x, y, z in toy:
        print(x, y, z)
else:
    print(-1)
