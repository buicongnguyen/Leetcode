---
description: Combine key lookup, frequency buckets, and recency tie-breaking for an LFU cache.
sample_status: conceptual
---

# LFU cache

LFU eviction chooses the key with the smallest access frequency. When multiple
keys share that frequency, it evicts the least-recently used among them.

## Why LRU is not enough

One global recency list orders by time only. LFU needs a two-level order:

1. smallest frequency;
2. oldest key inside that frequency.

## Ownership model

| Required fact | Owner |
| --- | --- |
| value and current frequency for a key | `key → node` hash map |
| recency among equal-frequency keys | one doubly linked list per frequency |
| direct access to a frequency list | `frequency → list` hash map |
| globally smallest live frequency | `min_frequency` scalar |

**Invariant:** each key appears in exactly one frequency list matching the
frequency stored in its node. `min_frequency` names the smallest nonempty
bucket.

## Access update order

```text
touch(key):
    node = key_map[key]
    old = node.frequency
    remove node from buckets[old]               # leave old ownership first

    if buckets[old] is empty:
        erase buckets[old]
        if min_frequency == old:
            min_frequency += 1                  # this access rises by one

    node.frequency += 1
    insert node at front of buckets[old + 1]    # newest in new frequency
```

For a new key, set frequency to one and reset `min_frequency = 1`.

For eviction, remove the tail of `buckets[min_frequency]`, then erase the same
key from the key map.

## Complexity and traps

- Average `O(1)` for `get` and `put` with hash maps and linked lists.
- Capacity zero must retain nothing.
- Updating an existing key counts as an access under the usual problem
  contract.
- Remove empty frequency buckets.
- Do not increment `min_frequency` after arbitrary eviction; reset it to one
  for insertion, or advance it during a touch only under the proven condition.

## Practice

[LeetCode: LFU Cache](https://leetcode.com/problems/lfu-cache/)

!!! note "Implementation status"

    This is a design blueprint. Add paired behavior tests before treating an
    implementation as copy-ready.
