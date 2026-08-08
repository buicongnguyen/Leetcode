---
description: Choose among linked lists, greedy proofs, bit operations, and string matching from problem signals.
sample_status: tested
---

# Core interview toolkit

These topics are small enough to hide inside larger problems. Choose them from
the operation and proof you need, not from the nouns in the story.

## Thinking flow · recognize the missing primitive

```mermaid
flowchart TD
  accTitle: Choosing a core interview technique
  accDescr: Pointer rewiring suggests linked lists, a locally safe irreversible choice suggests greedy, flags and subsets packed into integers suggest bit operations, and repeated prefix fallback suggests a string matching algorithm.
  A{"What operation dominates?"}
  A -->|"rewire nodes without shifting data"| B["Linked-list pointers"]
  A -->|"commit one choice and discard alternatives"| C{"Can an exchange argument prove safety?"}
  C -->|yes| D["Greedy"]
  C -->|no| E["DP, search, or flow"]
  A -->|"pack flags, masks, or subsets"| F["Bit manipulation"]
  A -->|"match a pattern while reusing prefix work"| G["KMP or another string index"]
```

## One pointer invariant to memorize

When reversing a singly linked list, the list is split into a reversed prefix
and an unread suffix. Save the suffix before overwriting `next`.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/core.py:reverse-linked-list"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:reverse-linked-list"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:reverse-linked-list"
    ```

## Reading path

1. [Linked lists](linked_lists.md): draw ownership before changing pointers.
2. [Greedy algorithms](greedy.md): prove a local choice with exchange or stay-ahead reasoning.
3. [Bit manipulation](bit_manipulation.md): use masks only when the representation simplifies state.
4. [String matching](string_matching.md): choose KMP, Z, Trie, or rolling hash from the query shape.

Continue to the [problem catalog](../appendix/problem_catalog.md) to practice by
recognition signal instead of random difficulty.
