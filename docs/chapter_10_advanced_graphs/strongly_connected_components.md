---
description: Collapse mutual-reachability groups into a DAG with Kosaraju or Tarjan SCC.
sample_status: conceptual
---

# Strongly connected components

In a directed graph, vertices belong to the same strongly connected component
when each can reach the other. SCC decomposition replaces every maximal mutual
reachability group with one component.

## Recognition signals

- “These states can all return to one another.”
- Group accounts, pages, or transitions into mutual-reachability regions.
- Detect cycles at component scale.
- Reduce a directed graph to a DAG before dynamic programming.
- Solve implication systems such as 2-SAT.

## Why the condensation graph matters

Collapse each SCC into one super-vertex. Any cycle between super-vertices would
mean those vertices were mutually reachable and should have been one SCC.
Therefore the condensation graph is always a DAG.

This often changes the real problem from “work on a cyclic directed graph” to
“work on a DAG of components.”

## Kosaraju thinking flow

```text
1. Build graph and reverse graph.
2. DFS the reverse graph; append each vertex after its descendants.
3. Process vertices in reverse finishing order on the original graph.
4. Every new DFS reached in step 3 is one complete SCC.
```

The first pass chooses an order in which the second pass cannot leak from an
unassigned component into one that should be processed later.

## Tarjan alternative

Tarjan uses one DFS, a stack, discovery indices, and low-link values.
When `low[u] == discovery[u]`, `u` is the root of an SCC; pop the stack through
`u`.

Choose Kosaraju when two passes and a reverse graph make the proof easier.
Choose Tarjan when a one-pass implementation or lower constant memory is
important and you are comfortable with its stack invariant.

## Complexity and traps

- Time: `O(V + E)`.
- Space: `O(V + E)` for adjacency and bookkeeping.
- SCC is for directed graphs; undirected connected components are simpler.
- “In the DFS stack” is different from “visited.”
- After assigning component IDs, deduplicate condensation edges when their
  multiplicity does not matter.
- Recursive DFS may overflow on a deep graph; use an explicit stack when input
  depth is unbounded.

## Reference

- [Princeton: Directed Graphs and strong connectivity](https://algs4.cs.princeton.edu/42digraph/)
- [Princeton: Tarjan SCC API](https://algs4.cs.princeton.edu/code/javadoc/edu/princeton/cs/algs4/TarjanSCC.html)

!!! note "Implementation status"

    SCC decomposition remains explanatory until the paired implementations
    receive behavior and deep-graph tests.
