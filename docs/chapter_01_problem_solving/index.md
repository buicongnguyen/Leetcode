---
description: A repeatable workflow for turning a LeetCode prompt into a defensible algorithm.
---

# Chapter 1 · Solve before coding

The story in a problem is often camouflage. Translate it into structure,
operations, and constraints before naming an algorithm.

## The eight-step loop

```mermaid
flowchart LR
  accTitle: The eight-step problem-solving loop
  accDescr: Restate the problem, bound the affordable work, model the state, find repeated waste, select a pattern, write its invariant, implement it, then test and audit the result.
  A["Restate"] --> B["Bound"]
  B --> C["Model"]
  C --> D["Find waste"]
  D --> E["Select pattern"]
  E --> F["Write invariant"]
  F --> G["Implement"]
  G --> H["Test and audit"]
```

### 1. Restate

Write one sentence containing the input, output, and validity rule. Clarify
duplicates, mutation, empty input, and whether a valid answer must exist.

### 2. Bound

Convert the largest input into a rough operation budget:

| Maximum `n` | First family to consider |
| ---: | --- |
| `20` | exponential search, bitmask DP |
| `200` | cubic, depending on constant factors |
| `2,000` | quadratic |
| `100,000` | `O(n log n)` or `O(n)` |
| `1,000,000` | linear or near-linear |

These are interview heuristics, not hardware guarantees.

### 3. Model

Ask what carries the state:

- sequence or contiguous range;
- tree or hierarchy;
- graph of relationships;
- grid of implicit neighbors;
- subset, position, or capacity state.

### 4. Find waste

Start from brute force. Circle the repeated scan, repeated subproblem, repeated
minimum lookup, or repeated connectivity query. The bottleneck often names the
data structure.

### 5. Select a pattern

The earlier table was directionally correct, but a signal alone is not enough.
Each pattern also has a condition that makes its discard or reuse step valid:

| Repeated need | First candidate | Valid when | Do not use it blindly when |
| --- | --- | --- | --- |
| membership, count, complement | hash map / set | order is not needed for the lookup | worst-case hashing or sorted iteration is part of the contract |
| next best live candidate | heap | candidates arrive, expire, or change dynamically | the data is static and sorting once is simpler |
| oldest pending state | queue | work must be processed in arrival or distance-layer order | priority, not age, decides what leaves next |
| newest unresolved state | stack | nested or last-in-first-out work must be resolved first | the earliest pending item owns the next answer |
| contiguous valid range | sliding window | moving a boundary changes validity monotonically | negative values or global constraints can make a boundary move backward |
| range aggregate or count | prefix sum | a range can be derived from two prefix states | updates occur between queries; consider a Fenwick or segment tree |
| ordered monotone decision | binary search | candidates are ordered and the predicate changes once | feasibility can switch back and forth |
| repeated complete state | dynamic programming | the state captures every future-relevant fact and dependencies terminate | histories with the same state variables can have different futures |
| cheapest transition sequence | BFS / shortest path | the problem is states plus legal moves | the structure is only a sequence with a simpler invariant |
| repeated connectivity merge | disjoint set | edges are added or processed offline and splits are not required | deletions or path details must be answered |

The first candidate should come from the expensive operation, not from a
memorized problem title:

```mermaid
flowchart TD
  accTitle: From repeated work to a first algorithm candidate
  accDescr: Identify the repeated expensive operation. Use hashing for repeated lookup, a heap for repeated best-candidate removal, a stack or queue when pending order matters, a window or prefix sum for contiguous ranges, binary search for a monotone boundary, dynamic programming for repeated states, and graph algorithms for transitions between states.
  A{"What work repeats?"}
  A -->|"membership, count,<br/>or complement lookup"| B["Hash map / set"]
  A -->|"remove the next<br/>best candidate"| C["Heap"]
  A -->|"process pending<br/>items by order"| D{"Which item leaves next?"}
  D -->|"newest"| E["Stack"]
  D -->|"oldest"| F["Queue"]
  D -->|"either end"| G["Deque"]
  A -->|"scan a contiguous<br/>range again"| H{"Can a boundary move<br/>without moving back?"}
  H -->|yes| I["Sliding window"]
  H -->|no| J["Prefix sum / offline query"]
  A -->|"test an ordered,<br/>monotone decision"| K["Binary search"]
  A -->|"solve the same state<br/>from many histories"| L["Dynamic programming"]
  A -->|"follow legal transitions<br/>between states"| M["Graph traversal / path algorithm"]
```

This map chooses a starting family. Constraints and a written invariant still
decide whether that candidate is valid.

#### The four selection gates

Before committing to the candidate, pass all four gates:

1. **Model:** name the sequence, state graph, tree, interval, or component.
2. **Operation:** name the repeated expensive action the structure makes cheap.
3. **Proof condition:** state the monotonicity, ordering, or state-reuse fact
   that permits discarding work.
4. **Budget:** verify that the number of states or operations fits the input
   bound.

For example, “subarray” does not automatically mean sliding window. With
nonnegative values, increasing the right boundary can only increase a sum, so
the left boundary can move forward monotonically. With negative values that
proof disappears; prefix sums, hashing, or another model may be required.

Likewise, “minimum” does not automatically mean heap. A heap is useful when the
candidate set changes and the next minimum must be requested repeatedly. For a
static collection, one minimum scan or one sort may be the clearer solution.

### 6. Write the invariant

An invariant is a testable statement, not a slogan. Prefer “every index before
`left` has been proved invalid” over “the pointers are correct.”

### 7. Implement

Choose variable names from the proof: `left`, `right`, `distance`,
`indegree`, `parent`, `best`. Hide mechanics in helpers only when the helper has
a crisp contract.

### 8. Test and audit

Trace:

- the smallest valid input;
- duplicates or equal values;
- a solution at each boundary;
- no solution, when allowed;
- maximum numeric values;
- a shape that drives worst-case behavior.

Finally recompute time and auxiliary space from the code actually written.

## Worked example: from waste to invariant

For Two Sum, brute force scans the remaining suffix for the needed complement
at every index. The repeated operation is membership lookup, so a hash map owns
that operation.

**Invariant:** before processing index `i`, the map contains one earlier index
for every value seen in `values[:i]`. Checking before inserting prevents an
element from matching itself.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/arrays.py:two-sum"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:two-sum"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:two-sum"
    ```

Trace `[3, 3]` with target `6`: the first `3` is stored, then the second finds
its complement at index `0`. This simultaneously tests duplicates and the
distinct-index rule.

!!! tip "Interview narration"

    State the brute force, name its repeated waste, explain which operation the
    chosen structure makes cheap, then state the invariant. This is clearer
    than guessing an algorithm name immediately.
