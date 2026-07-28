---
description: Fast mapping from problem signals to patterns, invariants, and costs.
---

# Quick reference

| Signal | Pattern | Invariant to write | Typical cost |
| --- | --- | --- | --- |
| complement / count | hash map | map describes processed prefix | `O(n)` expected |
| contiguous validity | sliding window | current window is valid/minimal | `O(n)` |
| next greater/smaller | monotonic stack | unresolved values are monotone | `O(n)` |
| fixed-window extrema | monotonic deque | live candidates ordered by value | `O(n)` |
| sorted exact lookup | binary search | target remains in interval | `O(log n)` |
| monotone feasibility | first-true search | false prefix excluded | `O(log range × check)` |
| unweighted shortest path | BFS | queue order is nondecreasing distance | `O(V+E)` |
| nonnegative weighted path | Dijkstra | non-stale pop is final | `O((V+E) log V)` |
| dependency order | topological sort | queue holds remaining zero-indegree nodes | `O(V+E)` |
| dynamic connectivity | DSU | each component has one root | near `O(1)` amortized |
| enumerate choices | backtracking | path equals current branch | output-dependent |
| overlapping subproblems | DP | state has exact declared meaning | states × transitions |

## Boundary audit

- empty and singleton input;
- duplicates and all-equal values;
- answer at the first or last position;
- no answer, if permitted;
- disconnected or cyclic graph;
- maximum numeric values and overflow;
- recursion depth;
- mutation of caller-owned input.

## Binary search contracts

```text
Exact match: [lo, hi], while lo <= hi, return index or -1
First true:  [lo, hi), while lo < hi, return lo (possibly n)
Answer:      [known lower, known feasible], return smallest feasible
```

## Graph chooser

```text
Traversal/reachability         -> DFS or BFS
Fewest equal-cost transitions  -> BFS
Weights in {0, 1}              -> 0–1 BFS
Nonnegative weights            -> Dijkstra
Negative edges                 -> Bellman–Ford
All pairs, small graph         -> Floyd–Warshall
Dependency ordering            -> topological sort
Dynamic connectivity / MST     -> DSU
```
