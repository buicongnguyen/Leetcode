"""Graph traversal, ordering, path, and connectivity algorithms."""

from collections import deque
from collections.abc import Sequence
from heapq import heappop, heappush
from math import inf


# --8<-- [start:bfs]
def bfs_distances(graph: Sequence[Sequence[int]], source: int) -> list[int]:
    """Return unweighted distances from ``source``; unreachable is ``-1``."""
    distance = [-1] * len(graph)
    distance[source] = 0
    queue = deque([source])
    while queue:
        node = queue.popleft()
        for neighbor in graph[node]:
            if distance[neighbor] == -1:
                distance[neighbor] = distance[node] + 1
                queue.append(neighbor)
    return distance
# --8<-- [end:bfs]


# --8<-- [start:dijkstra]
def dijkstra(
    graph: Sequence[Sequence[tuple[int, int]]], source: int
) -> list[float | int]:
    """Return shortest distances for a graph with nonnegative weights."""
    distance: list[float | int] = [inf] * len(graph)
    distance[source] = 0
    frontier: list[tuple[int, int]] = [(0, source)]

    while frontier:
        current, node = heappop(frontier)
        if current != distance[node]:
            continue
        for neighbor, weight in graph[node]:
            if weight < 0:
                raise ValueError("Dijkstra requires nonnegative weights")
            candidate = current + weight
            if candidate < distance[neighbor]:
                distance[neighbor] = candidate
                heappush(frontier, (candidate, neighbor))
    return distance
# --8<-- [end:dijkstra]


def topological_order(graph: Sequence[Sequence[int]]) -> list[int]:
    """Return a topological order or raise when the directed graph is cyclic."""
    indegree = [0] * len(graph)
    for neighbors in graph:
        for neighbor in neighbors:
            indegree[neighbor] += 1

    queue = deque(index for index, degree in enumerate(indegree) if degree == 0)
    order: list[int] = []
    while queue:
        node = queue.popleft()
        order.append(node)
        for neighbor in graph[node]:
            indegree[neighbor] -= 1
            if indegree[neighbor] == 0:
                queue.append(neighbor)

    if len(order) != len(graph):
        raise ValueError("directed graph contains a cycle")
    return order


def find_bridges(vertex_count: int, edges: Sequence[tuple[int, int]]) -> list[tuple[int, int]]:
    """Return bridge edges in an undirected multigraph."""
    graph: list[list[tuple[int, int]]] = [[] for _ in range(vertex_count)]
    for edge_id, (left, right) in enumerate(edges):
        graph[left].append((right, edge_id))
        graph[right].append((left, edge_id))

    entered = [-1] * vertex_count
    low = [0] * vertex_count
    timer = 0
    bridges: list[tuple[int, int]] = []

    def search(node: int, parent_edge: int = -1) -> None:
        nonlocal timer
        entered[node] = low[node] = timer
        timer += 1
        for neighbor, edge_id in graph[node]:
            if edge_id == parent_edge:
                continue
            if entered[neighbor] >= 0:
                low[node] = min(low[node], entered[neighbor])
                continue
            search(neighbor, edge_id)
            low[node] = min(low[node], low[neighbor])
            if low[neighbor] > entered[node]:
                bridges.append((min(node, neighbor), max(node, neighbor)))

    for vertex in range(vertex_count):
        if entered[vertex] < 0:
            search(vertex)
    return sorted(bridges)
