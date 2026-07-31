---
description: Use DFS discovery and low-link values to find single points of undirected connectivity failure.
sample_status: tested
---

# Bridges and articulation points

Use low-link DFS when an undirected graph asks which single edge or vertex can
disconnect connectivity.

- A **bridge** is an edge whose removal increases the number of components.
- An **articulation point** is a vertex whose removal increases the number of
  components.

## State meaning

- `tin[u]`: discovery time of `u`.
- `low[u]`: earliest discovery time reachable from `u`'s DFS subtree using
  tree edges plus at most one back edge.

For a DFS tree edge `u → v`:

- it is a bridge when `low[v] > tin[u]`;
- non-root `u` is an articulation point when `low[v] >= tin[u]` for at least
  one child `v`;
- a DFS root is an articulation point only when it has at least two DFS-tree
  children.

The strict versus non-strict comparison is deliberate. A back edge from the
child subtree to `u` protects the edge `u-v`, but removing `u` still separates
that subtree.

## Tested bridge template

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/graph.py:bridges"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:bridges"
    ```

## Why edge IDs are necessary

In a multigraph, two parallel edges connect the same endpoint pair. If DFS
skips every edge back to the parent vertex, it skips the parallel return edge
and may falsely report a bridge. Skip only the exact parent **edge ID**.

## Complexity and boundary tests

- Time: `O(V + E)`.
- Space: `O(V + E)` including adjacency.
- Run DFS from every unvisited vertex; the graph may be disconnected.
- Test one edge, a cycle, a tail attached to a cycle, parallel edges, and an
  isolated vertex.
- Consider iterative DFS or explicit depth constraints for very deep inputs.

## Practice

- [LeetCode: Critical Connections in a Network](https://leetcode.com/problems/critical-connections-in-a-network/)

Articulation-point extraction is described here but is not yet exposed as a
tested library function.
