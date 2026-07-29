---
description: Build state-space searches with choose, recurse, undo, and prune.
---

# Chapter 8 · Backtracking

Backtracking explores a decision tree while storing only the current path. Each
call owns a precise prefix of choices.

## Thinking flow · Backtracking or another model?

```mermaid
flowchart TD
  accTitle: Recognizing backtracking and choosing its decision shape
  accDescr: Use backtracking when the task must enumerate or construct choices and a branch can be undone. Use dynamic programming when different histories share the same future state, and BFS when the goal is the fewest legal moves. Within backtracking, choose take-skip for subsets, a start index for combinations, used markers for permutations, and one variable per level for constraint assignment.
  A{"What must the search return?"}
  A -->|"fewest legal transitions"| B["BFS / shortest path"]
  A -->|"best or count, with<br/>repeated future states"| C["Dynamic programming"]
  A -->|"one or all valid<br/>constructions"| D{"Can choices form a tree<br/>and be undone?"}
  D -->|no| E["Change the state model"]
  D -->|yes| F{"What does one level decide?"}
  F -->|"take or skip next item"| G["Subset tree"]
  F -->|"choose next from suffix"| H["Combination<br/>start index"]
  F -->|"choose any unused item"| I["Permutation<br/>used markers"]
  F -->|"assign next variable"| J["Constraint search<br/>validity checks"]
  G --> K["Add only proved pruning"]
  H --> K
  I --> K
  J --> K
```

The deciding distinction from DP is state reuse. Backtracking keeps separate
histories because their paths or assignments matter; DP merges histories only
when their remaining futures are identical.

```mermaid
flowchart TD
  accTitle: The choose, recurse, and undo backtracking cycle
  accDescr: From the current path, choose a candidate, recurse, undo that choice, and either try another candidate or return.
  A["Current path"] --> B["Choose candidate"]
  B --> C["Recurse"]
  C --> D["Undo choice"]
  D --> E{"More candidates?"}
  E -->|yes| B
  E -->|no| F["Return"]
```

The call cycle above is the mechanism. The explored search space is a tree. For
the choices `[1, 2]`, every level decides whether to take the next value:

```mermaid
flowchart TD
  accTitle: Backtracking decision tree for the subsets of one and two
  accDescr: Starting with an empty path, the first level takes or skips one and the second level takes or skips two, producing the four leaves one-two, one, two, and empty. Returning from a child undoes that branch's last choice before exploring its sibling.
  A["start: path = []"]
  A -->|"take 1"| B["path = [1]"]
  A -->|"skip 1"| C["path = []"]
  B -->|"take 2"| D["emit [1, 2]"]
  B -->|"skip 2"| E["emit [1]"]
  C -->|"take 2"| F["emit [2]"]
  C -->|"skip 2"| G["emit []"]
```

Siblings must begin from the same parent state. That is why the return from a
child and the undo operation belong together.

## Core invariant

`path` contains exactly the choices made on the current root-to-node branch.
The undo step must restore the state seen before the choice.

## Subsets with duplicates

Adjacent duplicate skipping has a precondition: candidates must be sorted.
Skip a duplicate only when it appears later in the *same decision layer*.

=== "Python"

    ```python
    --8<-- "codes/python/dsa_atlas/backtracking.py:subsets"
    ```

=== "C++17"

    ```cpp
    --8<-- "codes/cpp/include/dsa_atlas/algorithms.hpp:subsets"
    ```

The condition `i > start` preserves the ability to choose the same value at a
deeper layer while preventing identical sibling branches.

## Pruning

A prune is valid only when no completion below the current state can beat or
satisfy the answer. Common proofs use:

- a capacity already exceeded;
- a lower bound already worse than the best answer;
- remaining values unable to reach a target;
- a constraint violated by every extension.

Avoid changing shared state in the prune condition unless its restoration is
obvious.

## Complexity

State the number of candidate states and the work per emitted answer. For
subsets, there are `2^n` outputs and copying each path can make the total
`O(n 2^n)`.
