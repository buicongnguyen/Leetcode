---
description: Reason about linked-list ownership, pointer rewiring, sentinels, fast and slow pointers, and reversal.
sample_status: tested
---

# Linked lists

A linked list is useful when identity and rewiring matter more than indexed
access. Most bugs come from overwriting the only pointer to unread nodes.

## Draw roles, not variable names

```mermaid
flowchart LR
  accTitle: Reversing one linked-list edge safely
  accDescr: The previous pointer owns the reversed prefix, current is the node being moved, and following preserves access to the unread suffix before current next is reversed.
  P["previous<br/>reversed prefix"] <-- C["current"]
  C -. "save first" .-> N["following<br/>unread suffix"]
```

At the start of every iteration:

- `previous` heads a correctly reversed prefix;
- `current` heads the untouched suffix;
- the two regions contain every original node exactly once.

## Iterative reversal

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

Time is `O(n)` and auxiliary space is `O(1)`. Prefer iteration when recursion
would add one call frame per node.

## Pattern chooser

| Signal | Pattern | Proof checkpoint |
| --- | --- | --- |
| Delete/insert near the head | Dummy sentinel | The answer head is always `dummy.next` |
| Middle, cycle, or k-from-end | Slow/fast pointers | State the distance between pointers |
| Merge sorted lists | Tail builder | Tail always ends the merged prefix |
| Reverse a segment | Save four boundaries | Reconnect prefix, segment, and suffix |
| Cache with recency | List + hash map | Map and list own exactly the same live keys |

## Failure checks

- Save `next` before rewriting it.
- Decide whether the function may mutate the input.
- Test empty, one-node, two-node, and cyclic inputs separately.
- In C++, make node ownership explicit; this template rewires non-owning raw
  pointers and does not allocate or delete nodes.

## Practice ladder

1. [Reverse Linked List](https://leetcode.com/problems/reverse-linked-list/) — one invariant.
2. [Merge Two Sorted Lists](https://leetcode.com/problems/merge-two-sorted-lists/) — dummy sentinel.
3. [Linked List Cycle II](https://leetcode.com/problems/linked-list-cycle-ii/) — prove the meeting geometry.
4. [Reverse Nodes in k-Group](https://leetcode.com/problems/reverse-nodes-in-k-group/) — preserve group boundaries.
