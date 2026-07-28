"""Reusable composite data structures."""

from heapq import heappop, heappush


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
            self.parent[item] = self.parent[self.parent[item]]
            item = self.parent[item]
        return item

    def union(self, left: int, right: int) -> bool:
        left_root, right_root = self.find(left), self.find(right)
        if left_root == right_root:
            return False
        if self.component_size[left_root] < self.component_size[right_root]:
            left_root, right_root = right_root, left_root
        self.parent[right_root] = left_root
        self.component_size[left_root] += self.component_size[right_root]
        return True
# --8<-- [end:disjoint-set]


# --8<-- [start:median]
class MedianFinder:
    """Maintain the median of an online integer stream."""

    def __init__(self) -> None:
        self.lower: list[int] = []  # negated max-heap
        self.upper: list[int] = []  # min-heap

    def add(self, value: int) -> None:
        heappush(self.lower, -value)
        heappush(self.upper, -heappop(self.lower))
        if len(self.upper) > len(self.lower):
            heappush(self.lower, -heappop(self.upper))

    def median(self) -> float:
        if not self.lower:
            raise ValueError("median is undefined for an empty stream")
        if len(self.lower) > len(self.upper):
            return float(-self.lower[0])
        return (-self.lower[0] + self.upper[0]) / 2.0
# --8<-- [end:median]
