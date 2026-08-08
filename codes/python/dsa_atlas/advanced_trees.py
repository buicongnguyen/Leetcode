"""Prefix, range, and ordered-search structures."""

from __future__ import annotations

from dataclasses import dataclass, field


# --8<-- [start:trie]
@dataclass
class _TrieNode:
    children: dict[str, _TrieNode] = field(default_factory=dict)
    terminal: bool = False


class Trie:
    """Store strings by shared prefixes."""

    def __init__(self) -> None:
        self.root = _TrieNode()

    def insert(self, word: str) -> None:
        node = self.root
        for character in word:
            # Create only the missing suffix; existing prefixes are shared.
            node = node.children.setdefault(character, _TrieNode())
        node.terminal = True

    def contains(self, word: str) -> bool:
        node = self._walk(word)
        return node is not None and node.terminal

    def starts_with(self, prefix: str) -> bool:
        return self._walk(prefix) is not None

    def _walk(self, text: str) -> _TrieNode | None:
        node = self.root
        for character in text:
            node = node.children.get(character)
            if node is None:
                return None
        return node
# --8<-- [end:trie]


# --8<-- [start:fenwick-tree]
class FenwickTree:
    """Support point additions and prefix/range sums in logarithmic time."""

    def __init__(self, size: int) -> None:
        if size < 0:
            raise ValueError("size must be nonnegative")
        self.size = size
        self.tree = [0] * (size + 1)

    def add(self, index: int, delta: int) -> None:
        if not 0 <= index < self.size:
            raise IndexError("index outside Fenwick tree")
        cursor = index + 1
        while cursor <= self.size:
            self.tree[cursor] += delta
            # Add the lowest set bit to reach the next owning range.
            cursor += cursor & -cursor

    def prefix_sum(self, end: int) -> int:
        """Return the sum of the half-open prefix ``[0, end)``."""
        if not 0 <= end <= self.size:
            raise IndexError("prefix end outside Fenwick tree")
        total = 0
        cursor = end
        while cursor:
            total += self.tree[cursor]
            # Remove the lowest set bit to visit the parent prefix.
            cursor -= cursor & -cursor
        return total

    def range_sum(self, left: int, right: int) -> int:
        if not 0 <= left <= right <= self.size:
            raise IndexError("range outside Fenwick tree")
        return self.prefix_sum(right) - self.prefix_sum(left)
# --8<-- [end:fenwick-tree]


# --8<-- [start:segment-tree]
class SegmentTree:
    """Support point assignment and half-open range sums."""

    def __init__(self, values: list[int]) -> None:
        self.size = len(values)
        self.tree = [0] * (2 * self.size)
        self.tree[self.size :] = values
        for node in range(self.size - 1, 0, -1):
            # Each internal node owns the sum of its two children.
            self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1]

    def set(self, index: int, value: int) -> None:
        if not 0 <= index < self.size:
            raise IndexError("index outside segment tree")
        node = index + self.size
        self.tree[node] = value
        while node > 1:
            node //= 2
            self.tree[node] = self.tree[2 * node] + self.tree[2 * node + 1]

    def range_sum(self, left: int, right: int) -> int:
        if not 0 <= left <= right <= self.size:
            raise IndexError("range outside segment tree")
        left += self.size
        right += self.size
        total = 0
        while left < right:
            if left & 1:
                total += self.tree[left]
                left += 1
            if right & 1:
                right -= 1
                total += self.tree[right]
            # Move from the selected boundary nodes to their parents.
            left //= 2
            right //= 2
        return total
# --8<-- [end:segment-tree]
