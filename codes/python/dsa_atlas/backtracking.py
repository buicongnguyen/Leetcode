"""Backtracking patterns."""

from collections.abc import Sequence


# --8<-- [start:subsets]
def unique_subsets(values: Sequence[int]) -> list[list[int]]:
    """Return every distinct subset, including the empty subset."""
    # Sorting places equal sibling choices next to one another.
    candidates = sorted(values)
    answer: list[list[int]] = []
    path: list[int] = []

    def search(start: int) -> None:
        # Every current path is one valid subset.
        answer.append(path.copy())
        for index in range(start, len(candidates)):
            # Skip a duplicate only within this decision layer.
            if index > start and candidates[index] == candidates[index - 1]:
                continue
            # Choose, recurse, then undo before trying the next sibling.
            path.append(candidates[index])
            search(index + 1)
            path.pop()

    search(0)
    return answer
# --8<-- [end:subsets]
