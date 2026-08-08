"""Core interview structures and patterns that complement the first chapters."""

from __future__ import annotations

from dataclasses import dataclass
from collections.abc import Iterable, Sequence


# --8<-- [start:reverse-linked-list]
@dataclass
class ListNode:
    value: int
    next: ListNode | None = None


def reverse_linked_list(head: ListNode | None) -> ListNode | None:
    """Reverse a singly linked list in place and return its new head."""
    previous: ListNode | None = None
    current = head
    while current is not None:
        # Save the unread suffix before changing the only forward link.
        following = current.next
        current.next = previous
        # Advance both boundaries of the reversed prefix.
        previous, current = current, following
    return previous
# --8<-- [end:reverse-linked-list]


# --8<-- [start:interval-scheduling]
def max_non_overlapping_intervals(intervals: Iterable[tuple[int, int]]) -> int:
    """Return the largest compatible set of half-open intervals."""
    ordered = sorted(intervals, key=lambda interval: interval[1])
    selected = 0
    previous_end: int | None = None
    for start, end in ordered:
        if start > end:
            raise ValueError("interval start must not exceed end")
        if previous_end is None or start >= previous_end:
            # The earliest finishing compatible interval leaves most room.
            selected += 1
            previous_end = end
    return selected
# --8<-- [end:interval-scheduling]


# --8<-- [start:count-set-bits]
def count_set_bits(value: int) -> int:
    """Count one-bits in a nonnegative integer."""
    if value < 0:
        raise ValueError("value must be nonnegative")
    count = 0
    while value:
        # Clearing the lowest set bit makes one unit of progress.
        value &= value - 1
        count += 1
    return count
# --8<-- [end:count-set-bits]


# --8<-- [start:kmp-search]
def kmp_search(text: str, pattern: str) -> int:
    """Return the first pattern position in text, or ``-1``."""
    if not pattern:
        return 0

    longest_border = [0] * len(pattern)
    border = 0
    for index in range(1, len(pattern)):
        # Reuse the next-shorter border after a mismatch.
        while border and pattern[index] != pattern[border]:
            border = longest_border[border - 1]
        if pattern[index] == pattern[border]:
            border += 1
        longest_border[index] = border

    matched = 0
    for index, character in enumerate(text):
        while matched and character != pattern[matched]:
            matched = longest_border[matched - 1]
        if character == pattern[matched]:
            matched += 1
            if matched == len(pattern):
                # Convert the ending index into the match's start.
                return index - len(pattern) + 1
    return -1
# --8<-- [end:kmp-search]


def linked_values(head: ListNode | None) -> list[int]:
    """Return list values for examples and tests."""
    values: list[int] = []
    while head is not None:
        values.append(head.value)
        head = head.next
    return values
