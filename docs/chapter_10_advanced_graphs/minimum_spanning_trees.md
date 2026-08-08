---
description: Choose Kruskal or Prim, prove the greedy edge choice, and handle disconnected weighted graphs.
sample_status: tested
---

# Minimum spanning trees

Use a minimum spanning tree when the output must connect **every vertex** of an
undirected weighted graph with minimum total edge cost. It is not a
source-to-target shortest-path problem.

## Recognition checklist

- Every vertex must be connected.
- Any connected tree with `V` vertices uses exactly `V - 1` edges.
- The objective is the **sum of selected edges**, not a path distance.
- Edge weights may be zero or negative.
- If the graph is disconnected, decide whether the contract wants an error or
  a minimum spanning forest.

## Kruskal or Prim?

| Situation | Prefer | Owned operation |
| --- | --- | --- |
| edges already form a sortable list | Kruskal | next globally cheapest edge |
| sparse graph and DSU is convenient | Kruskal | reject component-forming cycles |
| graph is naturally an adjacency list | Prim | next cheapest edge crossing the tree frontier |
| dense graph with cheap vertex scans | eager Prim | cheapest connection for each outside vertex |

Both rely on the cut property: a minimum-weight edge crossing a cut is safe for
some minimum spanning tree.

## Kruskal thinking flow

1. Sort edges by increasing weight.
2. Let DSU represent the connected components formed by accepted edges.
3. Accept an edge only if its endpoints currently belong to different
   components.
4. Stop after accepting `V - 1` edges.
5. If fewer edges were accepted, the graph was disconnected.

**Invariant:** accepted edges are a forest that can still be extended to an
MST.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/structures.py:disjoint-set"
    --8<-- "codes/python/dsa_atlas/graph.py:kruskal"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:disjoint-set"
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:kruskal"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:disjoint-set"
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:kruskal"
    ```

## Worked example

For edges `(A,C,1)`, `(B,D,1)`, `(B,C,2)`, `(A,B,4)`, `(C,D,5)`:

1. accept `A-C`;
2. accept `B-D`;
3. accept `B-C`, which joins the two components;
4. stop at three edges for four vertices.

The total is `4`. The later edges are unnecessary even though they are valid
connections.

## Complexity and traps

- Kruskal: `O(E log E)` sorting plus near-constant amortized DSU operations.
- Heap-based Prim: `O(E log V)`.
- Do not require distinct weights; multiple MSTs may exist with the same total.
- Do not reject negative weights.
- Do not return a partial total as though it were a spanning tree.
- In C++, guard the accumulated weight from overflow.

## Practice and reference

- [Princeton: Minimum Spanning Trees](https://algs4.cs.princeton.edu/43mst/)
- [LeetCode: Min Cost to Connect All Points](https://leetcode.com/problems/min-cost-to-connect-all-points/)
