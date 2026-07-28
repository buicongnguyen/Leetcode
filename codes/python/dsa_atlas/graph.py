"""Graph traversal, ordering, path, and connectivity algorithms."""

from collections import deque
from collections.abc import Sequence
from heapq import heappop, heappush
from math import inf

from .structures import DisjointSet


def _validate_vertex(vertex: int, vertex_count: int, name: str) -> None:
    if not 0 <= vertex < vertex_count:
        raise ValueError(f"{name} must be a valid vertex")


def _validate_unweighted_graph(graph: Sequence[Sequence[int]]) -> None:
    for neighbors in graph:
        for neighbor in neighbors:
            _validate_vertex(neighbor, len(graph), "neighbor")


# --8<-- [start:bfs]
def bfs_distances(graph: Sequence[Sequence[int]], source: int) -> list[int]:
    """Return unweighted distances from ``source``; unreachable is ``-1``."""
    _validate_vertex(source, len(graph), "source")
    _validate_unweighted_graph(graph)
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
    _validate_vertex(source, len(graph), "source")
    for edges in graph:
        for neighbor, weight in edges:
            _validate_vertex(neighbor, len(graph), "neighbor")
            if weight < 0:
                raise ValueError("Dijkstra requires nonnegative weights")

    distance: list[float | int] = [inf] * len(graph)
    distance[source] = 0
    frontier: list[tuple[int, int]] = [(0, source)]

    while frontier:
        current, node = heappop(frontier)
        if current != distance[node]:
            continue
        for neighbor, weight in graph[node]:
            candidate = current + weight
            if candidate < distance[neighbor]:
                distance[neighbor] = candidate
                heappush(frontier, (candidate, neighbor))
    return distance
# --8<-- [end:dijkstra]


# --8<-- [start:topological-order]
def topological_order(graph: Sequence[Sequence[int]]) -> list[int]:
    """Return a topological order or raise when the directed graph is cyclic."""
    _validate_unweighted_graph(graph)
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
# --8<-- [end:topological-order]


# --8<-- [start:bridges]
def find_bridges(vertex_count: int, edges: Sequence[tuple[int, int]]) -> list[tuple[int, int]]:
    """Return bridge edges in an undirected multigraph."""
    if vertex_count < 0:
        raise ValueError("vertex_count must be nonnegative")
    graph: list[list[tuple[int, int]]] = [[] for _ in range(vertex_count)]
    for edge_id, (left, right) in enumerate(edges):
        _validate_vertex(left, vertex_count, "edge endpoint")
        _validate_vertex(right, vertex_count, "edge endpoint")
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
# --8<-- [end:bridges]


# --8<-- [start:kruskal]
def minimum_spanning_tree_weight(
    vertex_count: int, edges: Sequence[tuple[int, int, int]]
) -> int:
    """Return an MST weight or raise when the undirected graph is disconnected."""
    if vertex_count < 0:
        raise ValueError("vertex_count must be nonnegative")
    for left, right, _weight in edges:
        if not 0 <= left < vertex_count or not 0 <= right < vertex_count:
            raise ValueError("edge endpoint must be a valid vertex")

    groups = DisjointSet(vertex_count)
    total = 0
    accepted = 0
    for left, right, weight in sorted(edges, key=lambda edge: edge[2]):
        if groups.union(left, right):
            total += weight
            accepted += 1

    required = max(0, vertex_count - 1)
    if accepted != required:
        raise ValueError("undirected graph is disconnected")
    return total
# --8<-- [end:kruskal]
