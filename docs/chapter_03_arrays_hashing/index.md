---
description: Frequency maps, prefix sums, two pointers, and array invariants.
---

# Chapter 3 · Arrays and hashing

Arrays preserve order and give constant-time indexing. Hash tables trade order
for expected constant-time lookup. Many sequence problems become linear when a
repeated scan is replaced by remembered state.

## Thinking flow · Choose the sequence technique

```mermaid
flowchart TD
  accTitle: Choosing an array, hashing, prefix, pointer, or window technique
  accDescr: Choose hashing for repeated membership or frequency lookup, prefix state when a range can be derived from two prefixes, two pointers when sorted order proves a candidate can be discarded, and a sliding window when range validity changes monotonically as boundaries move. Use a monotonic deque when every fixed window needs an extreme.
  A{"What must be answered<br/>repeatedly?"}
  A -->|"membership, count,<br/>or complement"| B["Hash map / set"]
  A -->|"aggregate over a<br/>contiguous range"| C{"Can range value come from<br/>two prefix states?"}
  C -->|yes| D["Prefix sum / prefix count"]
  C -->|no| E{"Can a boundary move only<br/>forward as validity changes?"}
  E -->|yes| F["Sliding window"]
  E -->|no| G["Change the model;<br/>window is not proved"]
  A -->|"pair or interval relation<br/>in ordered data"| H{"Does one comparison safely<br/>discard one side?"}
  H -->|yes| I["Sort if allowed + two pointers"]
  H -->|no| J["Hashing, binary search,<br/>or another model"]
  A -->|"maximum/minimum of<br/>every fixed window"| K["Monotonic deque"]
```

The branch is valid only when its proof condition holds. “Subarray” does not
automatically imply sliding window, and “pair” does not automatically imply two
pointers.

## Complement lookup

For two-sum-style problems, scan once. Before storing the current value, ask
whether its complement appeared earlier.

**Invariant:** the map contains exactly the values strictly before the current
index.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/arrays.py:two-sum"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:two-sum"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:two-sum"
    ```

Checking before insertion prevents reusing the same element.

## Prefix-sum frequency

Let `prefix[j]` be the sum before position `j`. A subarray ending at the current
position sums to `target` when an earlier prefix equals
`current_prefix - target`.

```mermaid
flowchart LR
  accTitle: Prefix-sum lookup identity
  accDescr: The sum of the subarray from left to right is the current prefix minus an earlier prefix, so the algorithm looks for current prefix minus target.
  A["Earlier prefix<br/>prefix[left]"] -->|"subarray [left, right) adds target"| B["Current prefix<br/>prefix[right]"]
  B -. "look up prefix[right] - target" .-> A
```

**Invariant:** `seen` counts prefix sums from positions strictly before the
current endpoint.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/arrays.py:prefix-sum"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:prefix-sum"
    ```

=== "C++11"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:prefix-sum"
    ```

Initialize the empty prefix with count one. Without it, subarrays beginning at
index zero disappear.

## Two pointers

Two pointers work when pointer movement can permanently discard candidates.
In a sorted pair-sum search:

- a sum that is too small proves the current left value cannot work with any
  smaller right value;
- a sum that is too large proves the current right value cannot work with any
  larger left value.

If you cannot justify the discard, two pointers is only a guess.

## Frequency map checklist

- Does the answer depend on counts or only membership?
- Must you retain the earliest, latest, or every index?
- Can duplicates overwrite safely?
- Does the map represent the past only, or the full input?
- Is expected hash behavior acceptable?

## Practice ladder · name the remembered state

| Rung | Problem | Thinking prompt |
| --- | --- | --- |
| 1 | [Two Sum](https://leetcode.com/problems/two-sum/) | What does the map know before index `i`? |
| 2 | [Product of Array Except Self](https://leetcode.com/problems/product-of-array-except-self/) | Which directional aggregates exclude this value? |
| 3 | [Subarray Sum Equals K](https://leetcode.com/problems/subarray-sum-equals-k/) | Which earlier prefix completes the target? |
| 4 | [Longest Consecutive Sequence](https://leetcode.com/problems/longest-consecutive-sequence/) | Which value proves a sequence starts here? |

After each solution, record the map or set meaning in one sentence. The
[problem catalog](../appendix/problem_catalog.md) groups more problems by signal.
