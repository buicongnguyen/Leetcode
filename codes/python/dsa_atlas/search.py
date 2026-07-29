"""Binary-search contracts."""

from collections.abc import Callable, Sequence
from typing import TypeVar

T = TypeVar("T")


# --8<-- [start:binary-search]
def binary_search(values: Sequence[T], target: T) -> int:
    """Return any index containing ``target``, or ``-1`` when absent."""
    # Invariant: a present target remains inside inclusive [low, high].
    low, high = 0, len(values) - 1
    while low <= high:
        middle = low + (high - low) // 2
        if values[middle] == target:
            return middle
        if values[middle] < target:
            # middle and everything left of it are too small.
            low = middle + 1
        else:
            # middle and everything right of it are too large.
            high = middle - 1
    return -1
# --8<-- [end:binary-search]


# --8<-- [start:first-true]
def first_true(size: int, predicate: Callable[[int], bool]) -> int:
    """Return the first true index in ``[0, size)``, or ``size``."""
    if size < 0:
        raise ValueError("size must be nonnegative")
    # [low, high) always contains the first true position or sentinel size.
    low, high = 0, size
    while low < high:
        middle = low + (high - low) // 2
        if predicate(middle):
            # middle may be the answer, so keep it.
            high = middle
        else:
            # middle is false; discard the false prefix through middle.
            low = middle + 1
    return low
# --8<-- [end:first-true]


def lower_bound(values: Sequence[T], target: T) -> int:
    """Return the first index whose value is not less than ``target``."""
    return first_true(len(values), lambda index: values[index] >= target)
