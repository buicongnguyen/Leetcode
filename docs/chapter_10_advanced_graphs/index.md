---
description: Select advanced graph algorithms by objective, edge properties, and query pattern.
---

# Chapter 10 · Advanced graphs

Advanced graph selection starts with the result that must be proved.

| Objective | Key condition | Candidate |
| --- | --- | --- |
| connect all vertices cheaply | undirected weighted graph | [Kruskal / Prim MST](minimum_spanning_trees.md) |
| move maximum amount | capacities | [max flow](network_flow.md) |
| shortest paths with negative edges | reachable negative cycles matter | [Bellman–Ford](bellman_ford.md) |
| all-pairs paths, small `V` | dense or repeated queries | [Floyd–Warshall](floyd_warshall.md) |
| mutual reachability groups | directed graph | [SCC](strongly_connected_components.md) |
| critical undirected links | connectivity after removal | [bridge / articulation DFS](bridges_articulation_points.md) |
| repeated ancestor queries | static rooted tree | [LCA with binary lifting](lca_binary_lifting.md) |

```mermaid
flowchart TD
  accTitle: Choosing an advanced graph algorithm by the result to prove
  accDescr: Choose minimum spanning tree algorithms to connect all vertices cheaply, max flow to maximize movement through capacities, Bellman-Ford or Floyd-Warshall for specialized shortest paths, strongly connected components for mutual reachability, bridge search for critical links, and binary lifting for repeated ancestor queries.
  A{"What must the answer prove?"}
  A -->|"cheapest connection<br/>of all vertices"| B{"How are edges consumed?"}
  B -->|"sorted edge list"| C["Kruskal + DSU"]
  B -->|"grow from a frontier"| D["Prim + heap"]
  A -->|"maximum amount through<br/>capacity-limited edges"| E["Max flow<br/>Dinic / Edmonds–Karp"]
  A -->|"shortest paths with<br/>negative edges"| F["Bellman–Ford"]
  A -->|"all-pairs paths<br/>with small V"| G["Floyd–Warshall"]
  A -->|"mutual reachability<br/>groups"| H["Strongly connected<br/>components"]
  A -->|"links whose removal<br/>disconnects the graph"| I["Bridges / articulation points"]
  A -->|"many ancestor or<br/>path queries on a tree"| J["LCA / binary lifting"]
```

This selection begins with the output contract. Edge direction, weights,
capacities, graph size, and number of queries then remove invalid candidates.

## Detailed guides

Each guide separates recognition, proof, template, complexity, and failure
cases:

- [Minimum spanning trees](minimum_spanning_trees.md): cut property, Kruskal
  versus Prim, and disconnected inputs.
- [Network flow](network_flow.md): residual edges, augmenting paths, min-cut,
  and matching reductions.
- [Bellman–Ford](bellman_ford.md): negative edges and reachable negative-cycle
  detection.
- [Floyd–Warshall](floyd_warshall.md): the intermediate-vertex DP state for
  all-pairs paths.
- [Strongly connected components](strongly_connected_components.md):
  Kosaraju and the condensation DAG.
- [Bridges and articulation points](bridges_articulation_points.md): discovery
  times, low links, parallel edges, and root handling.
- [LCA and binary lifting](lca_binary_lifting.md): preprocessing a static tree
  for repeated ancestor queries.

## Tested example: minimum spanning tree

Kruskal sorts edges by weight and accepts an edge only when it joins different
components.

```mermaid
flowchart TD
  accTitle: Kruskal minimum-spanning-tree decision loop
  accDescr: Sort edges by increasing weight, inspect the next edge, skip it when both endpoints are already connected, otherwise unite the components and accept it, and stop after n minus one accepted edges.
  A["Sort edges by increasing weight"] --> B["Take next edge (u, v, weight)"]
  B --> C{"find(u) = find(v)?"}
  C -->|yes| D["Skip: it would create a cycle"]
  C -->|no| E["Union components<br/>and accept edge"]
  D --> F{"Accepted n - 1 edges?"}
  E --> F
  F -->|no| B
  F -->|yes| G["Minimum spanning tree complete"]
```

**Invariant:** accepted edges form a forest that can still be extended to a
minimum spanning tree.

The disjoint-set structure makes cycle checks nearly constant amortized time.
After processing, exactly `n - 1` accepted edges are required for a connected
graph.

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

The implementations validate every endpoint, accept negative edge weights, and
reject a disconnected graph instead of silently returning a forest. The
[complete MST guide](minimum_spanning_trees.md) explains when Prim is the
better representation.

!!! warning "Copy-ready policy"

    Pseudocode is labeled as explanation. Only implementations under `codes/`
    are promised to compile or import in the current release.

## Primary references

The selection table and complexity checks are cross-checked against Princeton's
[Algorithms and Data Structures Cheatsheet](https://algs4.cs.princeton.edu/cheatsheet/),
[minimum-spanning-tree chapter](https://algs4.cs.princeton.edu/43mst/),
[shortest-path chapter](https://algs4.cs.princeton.edu/44sp/), and
[directed-graph chapter](https://algs4.cs.princeton.edu/42digraph/).
