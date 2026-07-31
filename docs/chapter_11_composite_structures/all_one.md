---
description: Combine key lookup with an ordered linked list of count buckets for constant-time extrema.
sample_status: conceptual
---

# All O(1) data structure

The structure must increment and decrement string counts and return any key
with minimum or maximum count, all in expected constant time.

## Why a heap is awkward

A heap can expose one extreme, but changing a key's count leaves stale heap
entries or requires finding an arbitrary heap position. Two extrema make the
problem worse.

## Ownership model

- `key → bucket node` hash map finds a key's current count in `O(1)` expected.
- A doubly linked list orders **buckets** by count.
- Each bucket owns a hash set of keys with exactly that count.
- Head and tail sentinels expose minimum and maximum buckets.

**Invariant:** bucket counts strictly increase along the list, every bucket is
nonempty, and every live key appears in exactly one matching bucket.

## Increment blueprint

```text
increment(key):
    if key is new:
        ensure count-1 bucket exists after head
        place key there
    else:
        current = key_map[key]
        ensure current.count + 1 bucket exists after current
        move key to that next bucket
        remove current bucket if it becomes empty
```

Decrement mirrors this toward the previous bucket. When a count reaches zero,
remove the key instead of creating a zero bucket.

## Complexity and traps

- Expected `O(1)` for increment, decrement, minimum key, and maximum key.
- The linked list contains distinct **counts**, not individual keys.
- Create a bucket only when the adjacent count is missing.
- Remove empty buckets immediately.
- Update the key map to the destination bucket before discarding the source.

## Practice

[LeetCode: All O`one Data Structure](https://leetcode.com/problems/all-oone-data-structure/)

!!! note "Implementation status"

    This is a mutation-order blueprint. It needs paired tests before becoming a
    copy-ready template.
