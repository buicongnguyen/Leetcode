---
description: Define states, transitions, base cases, and evaluation order for dynamic programming.
---

# Chapter 9 · Dynamic programming

Dynamic programming is exhaustive search over a state graph with repeated
states evaluated once.

## The five-part definition

Before code, write:

1. **State:** exactly what `dp[...]` means.
2. **Transition:** which smaller states produce it.
3. **Base case:** the smallest solved states.
4. **Order:** why dependencies are ready when read.
5. **Answer:** which state or aggregate to return.

## Memoization or tabulation?

| Top-down memoization | Bottom-up tabulation |
| --- | --- |
| natural from recurrence | explicit evaluation order |
| visits reachable states | usually fills full state space |
| recursion overhead | easy space compression |
| good for sparse states | predictable memory access |

## 0/1 knapsack

For each item `(weight, value)`, update capacities in descending order.

**State:** `dp[c]` is the best value with capacity at most `c` using only items
processed so far.

**Why descending?** Reading `dp[c - weight]` must refer to the previous item
layer. Ascending order would permit the current item multiple times and solve
unbounded knapsack instead.

```mermaid
flowchart LR
  accTitle: Zero-one knapsack state dependency
  accDescr: The new value at capacity c is the better of skipping the item from old dp at c or taking the item from old dp at c minus its weight; updating capacities from high to low preserves both old values.
  A["old dp[c]"] -->|"skip item"| C["new dp[c]"]
  B["old dp[c - weight]"] -->|"+ item value"| C
  C --> D["write capacities<br/>from high to low"]
  D --> E["dp[c - weight] is still<br/>from the previous item layer"]
```

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/dp.py:knapsack"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:knapsack"
    ```

## State-design questions

- Which information from the past changes future legal moves?
- Can two histories with the same chosen state variables always share an
  optimal continuation?
- Is a dimension derivable from the others?
- Does the answer require exact capacity/length or at most capacity/length?
- Are impossible states represented distinctly from a zero-value state?

## Space compression warning

Compress only after identifying the dependency direction. A one-dimensional
array changes in place, so loop order becomes part of correctness, not merely
an optimization detail.
