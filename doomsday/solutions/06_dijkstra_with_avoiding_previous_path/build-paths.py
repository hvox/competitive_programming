#!/bin/env python3
import math
import sys
from collections import namedtuple
from pathlib import Path
from queue import PriorityQueue

sys.setrecursionlimit(5128)
Edge = namedtuple("Edge", "weight risks")

def get_weight_and_doomsday_scenarios(graph, path):
    path, scenarios, weight = iter(path), set(), 0
    u = next(path)
    for v in path:
        edge = graph[u][v]
        scenarios.update(edge.risks)
        weight += edge.weight
        u = v
    return weight, scenarios

def parse_input(source):
    lines = (
        (line for line in source.strip().split("\n") if line)
        if isinstance(source, str)
        else iter(source)
    )
    n, m, q = map(int, next(lines).split())
    graph = {u: {} for u in range(1, n + 1)}
    for _ in range(m):
        u, v, w, _, *cs = map(int, next(lines).split())
        edge = Edge(w, frozenset(cs))
        graph[u][v] = edge
        graph[v][u] = edge
    queris = [tuple(map(int, next(lines).split())) for _ in range(q)]
    return graph, queris


def dijkstra(G, s, t, risks):
    visited = {}
    Q = PriorityQueue()
    Q.put((0, s, None))
    while not Q.empty() and t not in visited:
        dist, u, prev = Q.get()
        if u not in visited:
            visited[u] = (dist, prev)
            for v in G[u]:
                w = sum(r in risks for r in graph[u][v].risks)
                if v not in visited:
                    Q.put((dist + w, v, u))
    if t not in visited:
        return None
    path = [t]
    while visited[path[-1]][-1]:
        path.append(visited[path[-1]][-1])
    return list(reversed(path))


source = (
    Path(sys.argv[1]).read_text()
    if len(sys.argv) > 1
    else (input() for _ in range(10**10))
)
graph, queris = parse_input(source)

solutions = []
for start, end in queris:
    path1 = dijkstra(graph, start, end, {})
    path2 = dijkstra(graph, start, end, get_weight_and_doomsday_scenarios(graph, path1)[1]) or path1
    solutions.append((path1, path2))

for solution in solutions:
    for path in solution:
        print(len(path) - 1, *path)
