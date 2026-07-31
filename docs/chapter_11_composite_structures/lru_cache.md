---
description: Combine direct lookup with recency order for a tested constant-time LRU cache.
sample_status: tested
---

# LRU cache

An LRU cache must support direct key lookup, refresh recency on access, and
evict the least-recent key at capacity. No single basic structure owns all
three operations.

## Operation contract

| Operation | Target | Owner |
| --- | ---: | --- |
| find key | `O(1)` expected | hash map `key → list node` |
| mark most recent | `O(1)` | doubly linked list splice |
| find least recent | `O(1)` | list tail |
| evict | `O(1)` expected | tail removal + map erase |

## Cross-structure invariant

Every live map entry points to exactly one live list node, every real list node
has exactly one map entry, the front is most recent, and the back is least
recent.

That sentence is the proof obligation for every mutation.

## Thinking flow

1. A map solves lookup but cannot answer which key is oldest.
2. A linked list owns recency but cannot find an arbitrary key quickly.
3. Store list-node handles in the map.
4. On successful `get`, move that existing node to the front.
5. On `put`, update-and-move or insert at the front.
6. If over capacity, capture the tail key, remove its node, and erase the same
   key from the map.

## Tested templates

The comments mark the synchronization points rather than narrating every
syntax line.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/structures.py:lru-cache"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:lru-cache"
    ```

## Boundary tests

- capacity zero;
- overwrite an existing key without creating a second node;
- `get` changes the next eviction victim;
- evict from capacity one;
- repeatedly update the current head or tail.

## Practice

[LeetCode: LRU Cache](https://leetcode.com/problems/lru-cache/)
