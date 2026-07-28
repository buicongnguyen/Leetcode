"""Backtracking patterns."""

from collections.abc import Sequence


# --8<-- [start:subsets]
def unique_subsets(values: Sequence[int]) -> list[list[int]]:
    """Return every distinct subset, including the empty subset."""
    candidates = sorted(values)
    answer: list[list[int]] = []
    path: list[int] = []

    def search(start: int) -> None:
        answer.append(path.copy())
        for index in range(start, len(candidates)):
            if index > start and candidates[index] == candidates[index - 1]:
                continue
            path.append(candidates[index])
            search(index + 1)
            path.pop()

    search(0)
    return answer
# --8<-- [end:subsets]
