"""Dynamic-programming patterns."""

from collections.abc import Iterable


# --8<-- [start:knapsack]
def knapsack_01(capacity: int, items: Iterable[tuple[int, int]]) -> int:
    """Return the maximum value when every item can be used at most once."""
    if capacity < 0:
        raise ValueError("capacity must be nonnegative")

    best = [0] * (capacity + 1)
    for weight, value in items:
        if weight <= 0:
            raise ValueError("item weights must be positive")
        # Descend so best[current - weight] still belongs to the prior item layer.
        for current in range(capacity, weight - 1, -1):
            best[current] = max(best[current], best[current - weight] + value)
    return best[capacity]
# --8<-- [end:knapsack]
