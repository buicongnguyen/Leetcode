"""Dynamic-programming patterns."""

from collections.abc import Iterable
from functools import cache


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


# --8<-- [start:non-adjacent]
def max_non_adjacent_sum(values: Iterable[int]) -> int:
    """Return the largest sum of non-adjacent values; the empty set is valid."""
    skip_previous = 0
    take_previous = 0
    for value in values:
        # Taking this value requires skipping the immediately previous value.
        take_current = skip_previous + value
        # Skipping this value preserves the best earlier result.
        skip_current = max(skip_previous, take_previous)
        take_previous, skip_previous = take_current, skip_current
    return max(skip_previous, take_previous)
# --8<-- [end:non-adjacent]


# --8<-- [start:lcs]
def longest_common_subsequence_length(left: str, right: str) -> int:
    """Return the length of a longest subsequence shared by both strings."""
    # Keep the shorter string on the columns to minimize memory.
    if len(left) < len(right):
        left, right = right, left

    previous = [0] * (len(right) + 1)
    for left_character in left:
        current = [0] * (len(right) + 1)
        for column, right_character in enumerate(right, start=1):
            if left_character == right_character:
                # Matching characters extend the answer for both shorter prefixes.
                current[column] = previous[column - 1] + 1
            else:
                # Otherwise discard one final character and keep the better option.
                current[column] = max(previous[column], current[column - 1])
        previous = current
    return previous[-1]
# --8<-- [end:lcs]


# --8<-- [start:coin-change]
def minimum_coins(coins: Iterable[int], amount: int) -> int:
    """Return the fewest reusable coins totaling amount, or -1 if impossible."""
    if amount < 0:
        raise ValueError("amount must be nonnegative")
    denominations = list(coins)
    if any(coin <= 0 for coin in denominations):
        raise ValueError("coin values must be positive")

    impossible = amount + 1
    best = [impossible] * (amount + 1)
    best[0] = 0  # Zero coins make amount zero.
    for current in range(1, amount + 1):
        for coin in denominations:
            if coin <= current:
                # Append this reusable coin to a solved smaller amount.
                best[current] = min(best[current], best[current - coin] + 1)
    return -1 if best[amount] == impossible else best[amount]
# --8<-- [end:coin-change]


# --8<-- [start:cherry-pickup]
def cherry_pickup_two_robots(grid: list[list[int]]) -> int:
    """Return the maximum cherries collected by two top-to-bottom robots."""
    if not grid or not grid[0]:
        return 0
    columns = len(grid[0])
    if any(len(row) != columns for row in grid):
        raise ValueError("grid must be rectangular")

    @cache
    def solve(row: int, first_column: int, second_column: int) -> int:
        if not (0 <= first_column < columns and 0 <= second_column < columns):
            return -10**18  # Invalid moves must never win a maximum.

        cherries = grid[row][first_column]
        if first_column != second_column:
            cherries += grid[row][second_column]  # Count a shared cell only once.
        if row == len(grid) - 1:
            return cherries

        best_suffix = -10**18
        for first_step in (-1, 0, 1):
            for second_step in (-1, 0, 1):
                # Both robot positions are required to describe the next state.
                best_suffix = max(
                    best_suffix,
                    solve(
                        row + 1,
                        first_column + first_step,
                        second_column + second_step,
                    ),
                )
        return cherries + best_suffix

    return solve(0, 0, columns - 1)
# --8<-- [end:cherry-pickup]


# --8<-- [start:digit-dp]
def count_distinct_digit_numbers(limit: int) -> int:
    """Count positive integers at most limit whose decimal digits are distinct."""
    if limit < 0:
        raise ValueError("limit must be nonnegative")
    digits = str(limit)

    @cache
    def solve(position: int, tight: bool, started: bool, used_mask: int) -> int:
        if position == len(digits):
            return int(started)  # Exclude the all-leading-zero representation.

        upper = int(digits[position]) if tight else 9
        total = 0
        for digit in range(upper + 1):
            next_tight = tight and digit == upper
            if not started and digit == 0:
                # Leading zeros do not consume digit zero.
                total += solve(position + 1, next_tight, False, used_mask)
            elif used_mask & (1 << digit) == 0:
                # The bitmask remembers every digit chosen for the real number.
                total += solve(
                    position + 1,
                    next_tight,
                    True,
                    used_mask | (1 << digit),
                )
        return total

    return solve(0, True, False, 0)
# --8<-- [end:digit-dp]
