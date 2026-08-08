---
description: Choose balanced search, prefix, range-query, or spatial trees from the query and update contract.
sample_status: tested
---

# Advanced trees

Tree names matter less than the order they preserve. Start from the operations:
ordered keys, shared prefixes, numeric ranges, or geometric regions.

## Thinking flow · choose the index

```mermaid
flowchart TD
  accTitle: Choosing an advanced tree structure
  accDescr: Ordered predecessor and successor queries suggest a balanced binary search tree, prefix queries suggest a Trie, point updates with prefix aggregates suggest a Fenwick tree, general associative ranges suggest a segment tree, and low-dimensional nearest or orthogonal spatial queries suggest a KD tree.
  A{"What is the query domain?"}
  A -->|"ordered comparable keys"| B["Balanced BST<br/>red-black / AVL"]
  A -->|"string or token prefixes"| C["Trie"]
  A -->|"numeric index ranges"| D{"Which operations?"}
  D -->|"point add + prefix/group range"| E["Fenwick tree"]
  D -->|"general associative range + updates"| F["Segment tree"]
  A -->|"low-dimensional points"| G["KD tree"]
```

## Smallest range template

A Fenwick tree is the compact first choice for point additions and prefix sums.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/advanced_trees.py:fenwick-tree"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:fenwick-tree"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:fenwick-tree"
    ```

## Reading path

- [Balanced search trees](balanced_search_trees.md) explains AVL versus
  red-black invariants and why standard-library ordered maps are usually safer.
- [Tries](tries.md) gives a tested prefix index.
- [Fenwick and segment trees](range_structures.md) compares range contracts.
- [KD trees](kd_trees.md) explains spatial partitioning and pruning limits.
