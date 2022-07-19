#!/bin/env python3
import math
import sys
from collections import namedtuple
from pathlib import Path

sys.setrecursionlimit(5128)
Edge = namedtuple("Edge", "weight risks")


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


def bfs(graph, start, end):
    reached = {}
    current = {start: None}
    while end not in reached:
        old_current, current = current, {}
        for u, prev in old_current.items():
            if u in reached:
                continue
            reached[u] = prev
            for v in graph[u]:
                current[v] = u
    path = [end]
    while reached[path[-1]]:
        path.append(reached[path[-1]])
    return list(reversed(path))


def dfs(graph, start, end):
    visited = set()

    def dfs(u):
        if u == end:
            return [u]
        if u in visited:
            return
        visited.add(u)
        for v in graph[u]:
            path = dfs(v)
            if path:
                path.append(u)
                return path

    return list(reversed(dfs(start)))


source = (
    Path(sys.argv[1]).read_text()
    if len(sys.argv) > 1
    else (input() for _ in range(10**10))
)
graph, queris = parse_input(source)

solutions = []
for start, end in queris:
    path = bfs(graph, start, end)
    solutions.append(tuple(f(graph, start, end) for f in (bfs, dfs)))

for solution in solutions:
    for path in solution:
        print(len(path) - 1, *path)
