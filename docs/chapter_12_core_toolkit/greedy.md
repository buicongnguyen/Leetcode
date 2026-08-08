---
description: Select greedy algorithms by exchange arguments, stay-ahead proofs, and interval structure.
sample_status: tested
---

# Greedy algorithms

Greedy is not “take the largest” or “sort and hope.” It is a proof that one
locally chosen action can be part of an optimal solution.

## Proof chooser

```mermaid
flowchart TD
  accTitle: Deciding whether a greedy choice is justified
  accDescr: Try an exchange argument when an optimal first choice can be swapped with the greedy choice, a stay-ahead proof when prefixes can be compared, and a cut property for spanning structures. If no proof survives a small counterexample, use dynamic programming or search.
  A["Propose a local choice"] --> B{"Can any optimum swap its first<br/>different choice with yours?"}
  B -->|yes| C["Exchange argument"]
  B -->|no| D{"Can every greedy prefix be<br/>shown at least as good?"}
  D -->|yes| E["Stay-ahead proof"]
  D -->|no| F{"Does a cut certify the<br/>cheapest safe connector?"}
  F -->|yes| G["Cut-property greedy"]
  F -->|no| H["Seek DP, flow, or search"]
```

## Interval scheduling

For the largest set of non-overlapping half-open intervals, choose the
compatible interval that ends earliest. Any optimum's first interval can be
replaced by this one without reducing space for later choices.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/core.py:interval-scheduling"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:interval-scheduling"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:interval-scheduling"
    ```

Sorting dominates at `O(n log n)` time; the scan is `O(n)`.

## Greedy or DP?

Use greedy when the exchange keeps every future option at least as good. Use DP
when the best next choice depends on remaining capacity, parity, history, or
another state dimension. Weighted interval scheduling is the classic warning:
earliest finish maximizes count, not total weight.

## Practice ladder

1. [Assign Cookies](https://leetcode.com/problems/assign-cookies/) — match two sorted frontiers.
2. [Non-overlapping Intervals](https://leetcode.com/problems/non-overlapping-intervals/) — earliest finish.
3. [Jump Game](https://leetcode.com/problems/jump-game/) — farthest reachable prefix.
4. [Gas Station](https://leetcode.com/problems/gas-station/) — discard impossible start prefixes.
