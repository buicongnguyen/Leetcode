"""Reusable composite data structures."""

from __future__ import annotations

from bisect import bisect_right
from heapq import heappop, heappush
from random import Random


# --8<-- [start:disjoint-set]
class DisjointSet:
    """Union-by-size disjoint set with path compression."""

    def __init__(self, size: int) -> None:
        if size < 0:
            raise ValueError("size must be nonnegative")
        self.parent = list(range(size))
        self.component_size = [1] * size

    def find(self, item: int) -> int:
        if not 0 <= item < len(self.parent):
            raise ValueError("item must be a valid set element")
        while self.parent[item] != item:
            # Point to the grandparent to shorten future find paths.
            self.parent[item] = self.parent[self.parent[item]]
            item = self.parent[item]
        return item

    def union(self, left: int, right: int) -> bool:
        left_root, right_root = self.find(left), self.find(right)
        if left_root == right_root:
            return False
        # Attach the smaller tree below the larger tree.
        if self.component_size[left_root] < self.component_size[right_root]:
            left_root, right_root = right_root, left_root
        self.parent[right_root] = left_root
        self.component_size[left_root] += self.component_size[right_root]
        return True
# --8<-- [end:disjoint-set]


# --8<-- [start:lru-cache]
class _LRUNode:
    """One entry in the recency list."""

    __slots__ = ("key", "value", "previous", "next")

    def __init__(self, key: int = 0, value: int = 0) -> None:
        self.key = key
        self.value = value
        self.previous: _LRUNode | None = None
        self.next: _LRUNode | None = None


class LRUCache:
    """Map keys to nodes while a linked list owns recency order."""

    def __init__(self, capacity: int) -> None:
        if capacity < 0:
            raise ValueError("capacity must be nonnegative")
        self.capacity = capacity
        self.nodes: dict[int, _LRUNode] = {}
        self.head, self.tail = _LRUNode(), _LRUNode()
        self.head.next, self.tail.previous = self.tail, self.head

    def _detach(self, node: _LRUNode) -> None:
        # Splice the node out without searching for its neighbors.
        assert node.previous is not None and node.next is not None
        node.previous.next = node.next
        node.next.previous = node.previous

    def _insert_front(self, node: _LRUNode) -> None:
        # The first real node is always the most recently used.
        first = self.head.next
        assert first is not None
        node.previous, node.next = self.head, first
        self.head.next = first.previous = node

    def _touch(self, node: _LRUNode) -> None:
        self._detach(node)
        self._insert_front(node)

    def get(self, key: int) -> int:
        node = self.nodes.get(key)
        if node is None:
            return -1
        # A successful read makes this entry most recent.
        self._touch(node)
        return node.value

    def put(self, key: int, value: int) -> None:
        if key in self.nodes:
            node = self.nodes[key]
            node.value = value
            self._touch(node)
            return

        node = _LRUNode(key, value)
        self.nodes[key] = node
        self._insert_front(node)
        if len(self.nodes) > self.capacity:
            # Evict the least-recent real node and its map entry together.
            oldest = self.tail.previous
            assert oldest is not None and oldest is not self.head
            self._detach(oldest)
            del self.nodes[oldest.key]
# --8<-- [end:lru-cache]


# --8<-- [start:randomized-set]
class RandomizedSet:
    """Combine an array for random access with a map for O(1) deletion."""

    def __init__(self, seed: int | None = None) -> None:
        self.values: list[int] = []
        self.position: dict[int, int] = {}
        self.random = Random(seed)

    def insert(self, value: int) -> bool:
        if value in self.position:
            return False
        self.position[value] = len(self.values)
        self.values.append(value)
        return True

    def remove(self, value: int) -> bool:
        if value not in self.position:
            return False
        index = self.position[value]
        last = self.values[-1]
        # Fill the removed slot with the last value before shrinking.
        self.values[index] = last
        self.position[last] = index
        self.values.pop()
        del self.position[value]
        return True

    def get_random(self) -> int:
        if not self.values:
            raise ValueError("cannot choose from an empty set")
        # Dense array indices make every live value equally reachable.
        return self.values[self.random.randrange(len(self.values))]
# --8<-- [end:randomized-set]


# --8<-- [start:median]
class MedianFinder:
    """Maintain the median of an online integer stream."""

    def __init__(self) -> None:
        self.lower: list[int] = []  # negated max-heap
        self.upper: list[int] = []  # min-heap

    def add(self, value: int) -> None:
        # Route through lower so every lower value stays <= every upper value.
        heappush(self.lower, -value)
        heappush(self.upper, -heappop(self.lower))
        if len(self.upper) > len(self.lower):
            # Keep lower the same size as upper or one element larger.
            heappush(self.lower, -heappop(self.upper))

    def median(self) -> float:
        if not self.lower:
            raise ValueError("median is undefined for an empty stream")
        if len(self.lower) > len(self.upper):
            return float(-self.lower[0])
        return (-self.lower[0] + self.upper[0]) / 2.0
# --8<-- [end:median]


# --8<-- [start:time-map]
class TimeMap:
    """Map each key to an append-only timestamp history."""

    def __init__(self) -> None:
        self.history: dict[str, list[tuple[int, str]]] = {}

    def set(self, key: str, value: str, timestamp: int) -> None:
        entries = self.history.setdefault(key, [])
        if entries and timestamp <= entries[-1][0]:
            raise ValueError("timestamps for one key must strictly increase")
        # Increasing timestamps keep the history sorted without extra work.
        entries.append((timestamp, value))

    def get(self, key: str, timestamp: int) -> str:
        entries = self.history.get(key, [])
        # upper_bound(timestamp) - 1 is the newest value not after the query.
        index = bisect_right(entries, timestamp, key=lambda entry: entry[0]) - 1
        return entries[index][1] if index >= 0 else ""
# --8<-- [end:time-map]


# --8<-- [start:min-stack]
class MinStack:
    """Store each value beside the minimum for its complete prefix."""

    def __init__(self) -> None:
        self.entries: list[tuple[int, int]] = []

    def push(self, value: int) -> None:
        current_minimum = value if not self.entries else min(value, self.entries[-1][1])
        # The saved prefix minimum restores automatically after a pop.
        self.entries.append((value, current_minimum))

    def pop(self) -> int:
        if not self.entries:
            raise IndexError("pop from empty MinStack")
        return self.entries.pop()[0]

    def top(self) -> int:
        if not self.entries:
            raise IndexError("top from empty MinStack")
        return self.entries[-1][0]

    def get_min(self) -> int:
        if not self.entries:
            raise IndexError("minimum of empty MinStack")
        return self.entries[-1][1]
# --8<-- [end:min-stack]


# --8<-- [start:number-containers]
class NumberContainers:
    """Map indices to numbers and numbers to lazy min-heaps of indices."""

    def __init__(self) -> None:
        self.index_value: dict[int, int] = {}
        self.value_indices: dict[int, list[int]] = {}

    def change(self, index: int, value: int) -> None:
        if index < 0:
            raise ValueError("index must be nonnegative")
        self.index_value[index] = value
        # Old heap entries remain; find removes them only when exposed.
        heappush(self.value_indices.setdefault(value, []), index)

    def find(self, value: int) -> int:
        candidates = self.value_indices.get(value, [])
        while candidates and self.index_value.get(candidates[0]) != value:
            heappop(candidates)
        return candidates[0] if candidates else -1
# --8<-- [end:number-containers]


# --8<-- [start:snapshot-array]
class SnapshotArray:
    """Store sparse per-index histories instead of copying every snapshot."""

    def __init__(self, length: int) -> None:
        if length < 0:
            raise ValueError("length must be nonnegative")
        self.current_snapshot = 0
        self.history = [[(0, 0)] for _ in range(length)]

    def set(self, index: int, value: int) -> None:
        if not 0 <= index < len(self.history):
            raise IndexError("index outside snapshot array")
        entries = self.history[index]
        if entries[-1][0] == self.current_snapshot:
            # Coalesce repeated writes in the current in-progress version.
            entries[-1] = (self.current_snapshot, value)
        else:
            entries.append((self.current_snapshot, value))

    def snap(self) -> int:
        snapshot = self.current_snapshot
        self.current_snapshot += 1
        return snapshot

    def get(self, index: int, snapshot: int) -> int:
        if not 0 <= index < len(self.history):
            raise IndexError("index outside snapshot array")
        if not 0 <= snapshot < self.current_snapshot:
            raise ValueError("snapshot has not been created")
        entries = self.history[index]
        # Right-bias finds the newest entry whose snapshot is not later.
        position = bisect_right(entries, (snapshot, float("inf"))) - 1
        return entries[position][1]
# --8<-- [end:snapshot-array]
