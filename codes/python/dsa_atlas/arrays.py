"""Array and hash-table patterns."""

from collections import defaultdict, deque
from collections.abc import Sequence


# --8<-- [start:two-sum]
def two_sum(values: Sequence[int], target: int) -> tuple[int, int] | None:
    """Return indices of two distinct values whose sum is ``target``."""
    position: dict[int, int] = {}
    for index, value in enumerate(values):
        complement = target - value
        if complement in position:
            return position[complement], index
        position[value] = index
    return None
# --8<-- [end:two-sum]


# --8<-- [start:prefix-sum]
def count_subarrays_sum(values: Sequence[int], target: int) -> int:
    """Count contiguous subarrays whose sum equals ``target``."""
    seen: dict[int, int] = defaultdict(int)
    seen[0] = 1
    prefix = answer = 0
    for value in values:
        prefix += value
        answer += seen[prefix - target]
        seen[prefix] += 1
    return answer
# --8<-- [end:prefix-sum]


# --8<-- [start:sliding-window]
def max_sliding_window(values: Sequence[int], window: int) -> list[int]:
    """Return the maximum in every complete fixed-size window."""
    if window <= 0 or window > len(values):
        raise ValueError("window must be between 1 and len(values)")

    candidates: deque[int] = deque()
    answer: list[int] = []
    for right, value in enumerate(values):
        while candidates and candidates[0] <= right - window:
            candidates.popleft()
        while candidates and values[candidates[-1]] <= value:
            candidates.pop()
        candidates.append(right)
        if right + 1 >= window:
            answer.append(values[candidates[0]])
    return answer
# --8<-- [end:sliding-window]
