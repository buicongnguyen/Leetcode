---
description: A tested, chapter-based LeetCode problem-solving handbook in C++17 and Python 3.
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

## The learning path

<div class="chapter-grid" markdown>

<a class="chapter-card" href="chapter_01_problem_solving/">
<small>01 · Method</small>
<strong>Solve before coding</strong>
<span>A repeatable loop from constraints to invariant.</span>
</a>

<a class="chapter-card" href="chapter_03_arrays_hashing/">
<small>03 · Sequences</small>
<strong>Arrays and hashing</strong>
<span>Turn repeated scans into constant-time memory.</span>
</a>

<a class="chapter-card" href="chapter_06_graphs/">
<small>06 · Relationships</small>
<strong>Graphs</strong>
<span>Model states and choose traversal by edge meaning.</span>
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

<a class="chapter-card" href="appendix/quick_reference/">
<small>Appendix</small>
<strong>Quick reference</strong>
<span>Signals, invariants, and complexity at a glance.</span>
</a>

</div>

## What makes this book different

- **Chapters explain decisions.** Templates are grouped by the reasoning that
  selects them, not just by data-structure name.
- **Code is real source.** C++17 compiles and Python imports; both are exercised
  by behavior tests.
- **Invariants come first.** Every core pattern states what must remain true as
  its loop or recursion advances.
- **The repository is the book.** Fixes to explanations, code, and tests travel
  together through review and deployment.

!!! note "Scope"

    This is a focused interview handbook, not an encyclopedia. Advanced
    algorithms enter the copy-ready library only after they receive executable
    tests.
