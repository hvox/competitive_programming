def gropolize(colors):
    global parents, sisters
    sibling_stack = [[]]
    stack, roots = [None], []
    for c in colors:
        if c == stack[-1]:
            stack.pop()
            sibling_stack.pop()
            if len(stack) == 1: roots.append(c)
            continue
        if c in parents and parents[c] != stack[-1]:
            return None
        parents[c] = stack[-1]
        for sibling in sibling_stack[-1]:
            sisters.setdefault(sibling, set()).add(c)
        sibling_stack[-1].append(c)
        stack.append(c)
        sibling_stack.append([c])
    if len(stack) > 1:
        return None
    return roots

def is_sublist(little, big):
    big = list(reversed(big))
    for x in little:
        while big and big[-1] != x:
            big.pop()
        if not big:
            return False
    return True

parents, sisters = {}, {}
n, m = map(int, input().split())
colorsets = [tuple(map(int, input().split()))[1:] for _ in range(m)]
deleted, prev_roots, prev_roots_set = set(), [], set()
deleted_colors = set()
prev_colors = set()
for colors in colorsets:
    roots = gropolize(colors)
    if roots is None:
        break
    if not is_sublist([r for r in roots if r in prev_roots_set], prev_roots):
        break
    roots_set = set(roots)
    colors_set = set(colors)
    if roots_set.intersection(deleted):
        break
    if colors_set.intersection(deleted_colors):
        break
    deleted.update(prev_roots_set - roots_set)
    deleted_colors.update(prev_colors - colors_set)
    prev_colors = colors_set
    prev_roots_set = roots_set
    prev_roots = roots
else:
    marked = {}
    def dfs(u):
        if u in marked:
            if marked[u] == 1:
                return True
            if marked[u] == 0:
                return False
        marked[u] = 0
        if u in sisters:
            for v in sisters[u]:
                if not dfs(v):
                    return False
        marked[u] = 1
        return True
    for u in range(n):
        if u not in marked:
            if not dfs(u):
                break
    else:
        print("YES")
        exit(0)
print("NO")
