---
description: A tested, chapter-based LeetCode problem-solving handbook in C++11, C++17, and Python 3.
---

# Turn problem signals into working code

DSA Atlas is an open handbook for the gap between *recognizing* an algorithm
and implementing it correctly under interview constraints.

<div class="hero-grid" markdown>

<div class="hero-copy" markdown>

Read the prompt as a set of operations. Bound those operations with the input
constraints. Choose a data structure that makes the repeated operation cheap.
Then write the invariant before the loop.

[Start with the solve loop](chapter_01_problem_solving/index.md){ .md-button .md-button--primary }
[Browse tested code](https://github.com/buicongnguyen/Leetcode/tree/main/codes){ .md-button }
[Explore System Design Atlas](https://buicongnguyen.github.io/SystemDesign/){ .md-button }

</div>

```text
signal      = "minimum steps"
edge_weight = 1

model       = graph(states, moves)
algorithm   = BFS
invariant   = "queue order is nondecreasing distance"

prove()
implement()
test_boundaries()
```

</div>

## A tested code pattern

When a prompt asks for two distinct values that reach a target, the brute-force
pair scan repeats the same complement search. A hash map turns that repeated
search into expected constant time while preserving the earlier index.

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

Every sample shown in the learning chapters comes from the source tree and is
exercised by the repository's behavior tests.

## The learning path

<div class="chapter-grid" markdown>

<a class="chapter-card" href="chapter_00_preface/">
<small>00 · Start</small>
<strong>Use this book</strong>
<span>Move between explanation, source, tests, and deliberate practice.</span>
</a>

<a class="chapter-card" href="chapter_01_problem_solving/">
<small>01 · Method</small>
<strong>Solve before coding</strong>
<span>A repeatable loop from constraints to invariant.</span>
</a>

<a class="chapter-card" href="chapter_02_complexity/">
<small>02 · Bounds</small>
<strong>Complexity and constraints</strong>
<span>Turn input limits into an algorithm budget.</span>
</a>

<a class="chapter-card" href="chapter_03_arrays_hashing/">
<small>03 · Sequences</small>
<strong>Arrays and hashing</strong>
<span>Turn repeated scans into constant-time memory.</span>
</a>

<a class="chapter-card" href="chapter_04_linear_structures/">
<small>04 · Order</small>
<strong>Stacks, queues, and windows</strong>
<span>Maintain only the candidates that can still matter.</span>
</a>

<a class="chapter-card" href="chapter_05_trees_heaps/">
<small>05 · Hierarchy</small>
<strong>Trees and heaps</strong>
<span>Write subtree contracts and manage the next extreme candidate.</span>
</a>

<a class="chapter-card" href="chapter_06_graphs/">
<small>06 · Relationships</small>
<strong>Graphs</strong>
<span>Model states and choose traversal by edge meaning.</span>
</a>

<a class="chapter-card" href="chapter_07_search_sort/">
<small>07 · Boundaries</small>
<strong>Search and sort</strong>
<span>Make the search contract explicit before moving either bound.</span>
</a>

<a class="chapter-card" href="chapter_08_backtracking/">
<small>08 · Choices</small>
<strong>Backtracking</strong>
<span>Explore, undo, and prune without losing the path invariant.</span>
</a>

<a class="chapter-card" href="chapter_09_dynamic_programming/">
<small>09 · State</small>
<strong>Dynamic programming</strong>
<span>Define the state before writing the recurrence.</span>
</a>

<a class="chapter-card" href="chapter_10_advanced_graphs/">
<small>10 · Advanced</small>
<strong>Structure and flow</strong>
<span>Shortest paths, connectivity, and capacity.</span>
</a>

<a class="chapter-card" href="chapter_11_composite_structures/">
<small>11 · Design</small>
<strong>Composite structures</strong>
<span>Assign each required operation to the structure that owns it.</span>
</a>

<a class="chapter-card" href="appendix/quick_reference/">
<small>Appendix</small>
<strong>Quick reference</strong>
<span>Signals, invariants, and complexity at a glance.</span>
</a>

</div>

## What makes this book different

- **Chapters explain decisions.** Templates are grouped by the reasoning that
  selects them, not just by data-structure name.
- **Code is real source.** C++11 and C++17 compile from the same templates, and
  Python imports; all are exercised by behavior tests.
- **Invariants come first.** Every core pattern states what must remain true as
  its loop or recursion advances.
- **The repository is the book.** Fixes to explanations, code, and tests travel
  together through review and deployment.

!!! note "Scope"

    This is a focused interview handbook, not an encyclopedia. Advanced
    algorithms enter the copy-ready library only after they receive executable
    tests.
