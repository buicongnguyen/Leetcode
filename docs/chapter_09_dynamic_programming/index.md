---
description: Define states, transitions, base cases, and evaluation order for dynamic programming.
---

# Chapter 9 · Dynamic programming

Dynamic programming is exhaustive search over a state graph with repeated
states evaluated once.

Two different histories may reach the same future. If the state description
contains everything that future decisions need, those histories can merge:

```mermaid
flowchart TD
  accTitle: Two histories merge into one dynamic-programming state
  accDescr: From grid position zero-zero, moving right then down and moving down then right both reach position one-one. Because the remaining problem is identical at one-one, dynamic programming evaluates that state once and reuses its answer.
  A["state (0, 0)"]
  A -->|"right"| B["state (0, 1)"]
  A -->|"down"| C["state (1, 0)"]
  B -->|"down"| D["state (1, 1)"]
  C -->|"right"| D
  D --> E["solve remaining suffix once"]
```

If two merged histories would have different legal moves or future value, the
state is missing a variable and must be refined.

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

```mermaid
flowchart TD
  accTitle: Choosing memoization, tabulation, and safe space compression
  accDescr: Prefer memoization for a natural recurrence with sparse reachable states when recursion depth is safe. Prefer tabulation for dense states, deep dependency chains, or a clear evaluation order. Compress memory only after proving that each state reads a limited set of earlier layers and choosing the safe update direction.
  A{"Are reachable states<br/>sparse or irregular?"}
  A -->|yes| B{"Is recursion depth<br/>provably safe?"}
  B -->|yes| C["Top-down memoization"]
  B -->|no| D["Iterative evaluation<br/>with an explicit order"]
  A -->|no| E{"Is dependency order<br/>easy to state?"}
  E -->|yes| F["Bottom-up tabulation"]
  E -->|no| G["Model dependencies as a DAG,<br/>then derive an order"]
  C --> H{"Need less memory?"}
  D --> H
  F --> H
  G --> H
  H -->|"depends on limited<br/>earlier layers"| I["Compress only after<br/>proving update direction"]
  H -->|"dependencies are broad"| J["Keep the full table"]
```

Memoization and tabulation evaluate the same recurrence. The choice changes
which states are visited, how evaluation order is enforced, and whether call
stack depth is part of the risk.

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
