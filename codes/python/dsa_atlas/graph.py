"""Graph traversal, ordering, path, and connectivity algorithms."""

from collections import deque
from collections.abc import Sequence
from dataclasses import dataclass
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
    # Mark on enqueue so each vertex enters the queue once.
    distance[source] = 0
    queue = deque([source])
    while queue:
        node = queue.popleft()
        for neighbor in graph[node]:
            if distance[neighbor] == -1:
                # Queue order guarantees this first distance is shortest.
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
        # Ignore heap entries superseded by a shorter route.
        if current != distance[node]:
            continue
        for neighbor, weight in graph[node]:
            candidate = current + weight
            if candidate < distance[neighbor]:
                # Relax the edge and schedule the improved state.
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

    # Only vertices with no unmet prerequisite are ready.
    queue = deque(index for index, degree in enumerate(indegree) if degree == 0)
    order: list[int] = []
    while queue:
        node = queue.popleft()
        order.append(node)
        for neighbor in graph[node]:
            indegree[neighbor] -= 1
            if indegree[neighbor] == 0:
                # Its final prerequisite has just been removed.
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
        # Edge IDs distinguish parallel edges from the exact parent edge.
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
                # A back edge may connect this subtree to an ancestor.
                low[node] = min(low[node], entered[neighbor])
                continue
            search(neighbor, edge_id)
            low[node] = min(low[node], low[neighbor])
            if low[neighbor] > entered[node]:
                # The child subtree has no route around this tree edge.
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
    # The cheapest safe edge is justified by the MST cut property.
    for left, right, weight in sorted(edges, key=lambda edge: edge[2]):
        if groups.union(left, right):
            total += weight
            accepted += 1

    required = max(0, vertex_count - 1)
    if accepted != required:
        raise ValueError("undirected graph is disconnected")
    return total
# --8<-- [end:kruskal]


# --8<-- [start:bellman-ford]
def bellman_ford(
    vertex_count: int, edges: Sequence[tuple[int, int, int]], source: int
) -> list[float | int]:
    """Return shortest distances, rejecting a reachable negative cycle."""
    _validate_vertex(source, vertex_count, "source")
    for left, right, _weight in edges:
        _validate_vertex(left, vertex_count, "edge endpoint")
        _validate_vertex(right, vertex_count, "edge endpoint")

    distance: list[float | int] = [inf] * vertex_count
    distance[source] = 0
    for _ in range(vertex_count - 1):
        changed = False
        for left, right, weight in edges:
            if distance[left] != inf and distance[left] + weight < distance[right]:
                # Relax only from a vertex the source can reach.
                distance[right] = distance[left] + weight
                changed = True
        if not changed:
            break

    for left, right, weight in edges:
        if distance[left] != inf and distance[left] + weight < distance[right]:
            raise ValueError("reachable negative cycle")
    return distance
# --8<-- [end:bellman-ford]


# --8<-- [start:floyd-warshall]
def floyd_warshall(matrix: Sequence[Sequence[float | int]]) -> list[list[float | int]]:
    """Return all-pairs shortest distances from an adjacency matrix."""
    size = len(matrix)
    if any(len(row) != size for row in matrix):
        raise ValueError("distance matrix must be square")
    distance = [list(row) for row in matrix]
    for vertex in range(size):
        distance[vertex][vertex] = min(distance[vertex][vertex], 0)

    for middle in range(size):
        for left in range(size):
            if distance[left][middle] == inf:
                continue
            for right in range(size):
                if distance[middle][right] == inf:
                    continue
                through_middle = distance[left][middle] + distance[middle][right]
                if through_middle < distance[left][right]:
                    # After phase middle, only vertices 0..middle may be internal.
                    distance[left][right] = through_middle

    if any(distance[vertex][vertex] < 0 for vertex in range(size)):
        raise ValueError("graph contains a negative cycle")
    return distance
# --8<-- [end:floyd-warshall]


# --8<-- [start:scc]
def strongly_connected_components(graph: Sequence[Sequence[int]]) -> list[list[int]]:
    """Partition a directed graph with iterative Kosaraju traversal."""
    _validate_unweighted_graph(graph)
    visited = [False] * len(graph)
    finish_order: list[int] = []

    for start in range(len(graph)):
        if visited[start]:
            continue
        visited[start] = True
        stack = [(start, 0)]
        while stack:
            node, edge_index = stack[-1]
            if edge_index < len(graph[node]):
                neighbor = graph[node][edge_index]
                stack[-1] = (node, edge_index + 1)
                if not visited[neighbor]:
                    visited[neighbor] = True
                    stack.append((neighbor, 0))
            else:
                # Postorder records a vertex only after its suffix is complete.
                finish_order.append(node)
                stack.pop()

    reversed_graph: list[list[int]] = [[] for _ in graph]
    for node, neighbors in enumerate(graph):
        for neighbor in neighbors:
            reversed_graph[neighbor].append(node)

    component_of = [-1] * len(graph)
    components: list[list[int]] = []
    for start in reversed(finish_order):
        if component_of[start] != -1:
            continue
        component_id = len(components)
        component: list[int] = []
        stack = [start]
        component_of[start] = component_id
        while stack:
            node = stack.pop()
            component.append(node)
            for neighbor in reversed_graph[node]:
                if component_of[neighbor] == -1:
                    component_of[neighbor] = component_id
                    stack.append(neighbor)
        components.append(sorted(component))
    return components
# --8<-- [end:scc]


# --8<-- [start:binary-lifting]
class BinaryLiftingTree:
    """Answer lowest-common-ancestor queries on a fixed undirected tree."""

    def __init__(self, tree: Sequence[Sequence[int]], root: int = 0) -> None:
        _validate_vertex(root, len(tree), "root")
        _validate_unweighted_graph(tree)
        neighbor_sets = [set(neighbors) for neighbors in tree]
        if any(
            len(neighbors) != len(neighbor_sets[node])
            for node, neighbors in enumerate(tree)
        ):
            raise ValueError("input tree cannot contain parallel edges")
        if any(node in neighbors for node, neighbors in enumerate(neighbor_sets)):
            raise ValueError("input tree cannot contain self-loops")
        if sum(len(neighbors) for neighbors in tree) != 2 * (len(tree) - 1):
            raise ValueError("input must contain exactly n - 1 undirected edges")
        for node, neighbors in enumerate(neighbor_sets):
            if any(node not in neighbor_sets[neighbor] for neighbor in neighbors):
                raise ValueError("every tree edge must appear in both directions")
        self.depth = [0] * len(tree)
        parent = [-1] * len(tree)
        parent[root] = root
        stack = [root]
        while stack:
            node = stack.pop()
            for neighbor in tree[node]:
                if neighbor == parent[node]:
                    continue
                if parent[neighbor] != -1:
                    raise ValueError("input must be an undirected tree")
                parent[neighbor] = node
                self.depth[neighbor] = self.depth[node] + 1
                stack.append(neighbor)
        if any(ancestor == -1 for ancestor in parent):
            raise ValueError("input tree must be connected")

        levels = max(1, len(tree).bit_length())
        self.up = [parent]
        for _ in range(1, levels):
            previous = self.up[-1]
            # Doubling composes two jumps of length 2^(level - 1).
            self.up.append([previous[previous[node]] for node in range(len(tree))])

    def lca(self, left: int, right: int) -> int:
        _validate_vertex(left, len(self.depth), "left")
        _validate_vertex(right, len(self.depth), "right")
        if self.depth[left] < self.depth[right]:
            left, right = right, left

        difference = self.depth[left] - self.depth[right]
        for level in range(len(self.up)):
            if difference & (1 << level):
                left = self.up[level][left]
        if left == right:
            return left

        for level in range(len(self.up) - 1, -1, -1):
            if self.up[level][left] != self.up[level][right]:
                # Lift together while their 2^level ancestors still differ.
                left = self.up[level][left]
                right = self.up[level][right]
        return self.up[0][left]
# --8<-- [end:binary-lifting]


# --8<-- [start:dinic]
@dataclass
class _FlowEdge:
    to: int
    reverse: int
    capacity: int


class Dinic:
    """Compute maximum flow with level graphs and blocking-flow DFS."""

    def __init__(self, vertex_count: int) -> None:
        if vertex_count < 0:
            raise ValueError("vertex_count must be nonnegative")
        self.graph: list[list[_FlowEdge]] = [[] for _ in range(vertex_count)]

    def add_edge(self, left: int, right: int, capacity: int) -> None:
        _validate_vertex(left, len(self.graph), "edge endpoint")
        _validate_vertex(right, len(self.graph), "edge endpoint")
        if capacity < 0:
            raise ValueError("capacity must be nonnegative")
        forward = _FlowEdge(right, len(self.graph[right]), capacity)
        backward = _FlowEdge(left, len(self.graph[left]), 0)
        self.graph[left].append(forward)
        self.graph[right].append(backward)

    def max_flow(self, source: int, sink: int) -> int:
        _validate_vertex(source, len(self.graph), "source")
        _validate_vertex(sink, len(self.graph), "sink")
        if source == sink:
            return 0
        total = 0
        while True:
            level = [-1] * len(self.graph)
            level[source] = 0
            queue = deque([source])
            while queue:
                node = queue.popleft()
                for edge in self.graph[node]:
                    if edge.capacity > 0 and level[edge.to] == -1:
                        level[edge.to] = level[node] + 1
                        queue.append(edge.to)
            if level[sink] == -1:
                return total

            next_edge = [0] * len(self.graph)

            def push(node: int, available: int) -> int:
                if node == sink:
                    return available
                while next_edge[node] < len(self.graph[node]):
                    edge = self.graph[node][next_edge[node]]
                    if edge.capacity > 0 and level[edge.to] == level[node] + 1:
                        sent = push(edge.to, min(available, edge.capacity))
                        if sent:
                            # Forward and reverse residual capacities change together.
                            edge.capacity -= sent
                            self.graph[edge.to][edge.reverse].capacity += sent
                            return sent
                    next_edge[node] += 1
                return 0

            while True:
                sent = push(source, 10**30)
                if not sent:
                    break
                total += sent
# --8<-- [end:dinic]


# --8<-- [start:a-star-grid]
def a_star_grid(
    grid: Sequence[Sequence[int]], start: tuple[int, int], goal: tuple[int, int]
) -> int:
    """Return a shortest four-direction path length through zero cells."""
    if not grid or not grid[0] or any(len(row) != len(grid[0]) for row in grid):
        raise ValueError("grid must be nonempty and rectangular")
    rows, columns = len(grid), len(grid[0])
    for row, column in (start, goal):
        if not 0 <= row < rows or not 0 <= column < columns:
            raise ValueError("endpoint outside grid")
        if grid[row][column] != 0:
            return -1

    best = {start: 0}
    frontier = [(abs(start[0] - goal[0]) + abs(start[1] - goal[1]), 0, start)]
    while frontier:
        _estimate, distance, cell = heappop(frontier)
        if distance != best[cell]:
            continue
        if cell == goal:
            return distance
        row, column = cell
        for row_step, column_step in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            neighbor = (row + row_step, column + column_step)
            if not (0 <= neighbor[0] < rows and 0 <= neighbor[1] < columns):
                continue
            if grid[neighbor[0]][neighbor[1]] != 0:
                continue
            candidate = distance + 1
            if candidate < best.get(neighbor, inf):
                best[neighbor] = candidate
                # Manhattan distance is admissible for four-direction unit moves.
                heuristic = abs(neighbor[0] - goal[0]) + abs(neighbor[1] - goal[1])
                heappush(frontier, (candidate + heuristic, candidate, neighbor))
    return -1
# --8<-- [end:a-star-grid]
