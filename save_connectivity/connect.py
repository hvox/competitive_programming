def f(G):
    edges, visited = set(), set()
    def dfs(u):
        if u in visited:
            return False
        visited.add(u)
        for v in G[u]:
            if dfs(v):
                edges.add((u, v))
        return True
    dfs(next(iter(G)))
    return edges

print("Number of vertices and edges: ")
n, m = map(int, input().split())
G = {u:set() for u in range(1, n + 1)}
Gr = {u:set() for u in range(1, n+1)}
edges = set()
print("Edges/")
for _ in range(m):
    u, v = map(int, input().split())
    G[u].add(v)
    Gr[v].add(u)
    edges.add((u, v))
required_edges = f(G) | f(Gr)
deleted_edges = edges - required_edges
print("deleted edges")
for edge, _ in zip(deleted_edges, range(m - 2*n)):
    print(" ".join(map(str, edge)))
