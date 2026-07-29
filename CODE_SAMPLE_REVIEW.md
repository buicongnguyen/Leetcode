# Code-sample coverage review

## Goal

Every learning chapter should connect reasoning to code that is visible in the
book, stored in the repository, and exercised by tests. Pseudocode remains
useful for decision flows, but it does not count as a copy-ready sample.

## Findings and resolution

| Surface | Before | Resolution |
| --- | --- | --- |
| Home | One pseudocode block and six chapter cards | Added a paired tested example and the complete chapter map. |
| Chapter 1 | Method only | Added a worked Two Sum example that derives the map invariant from repeated waste. |
| Chapter 5 | Tree-height prose only | Added tested `TreeNode` and `tree_height` implementations in both languages. |
| Chapter 6 | Topological sort and bridges were described but hidden in source | Exposed both tested implementations as paired samples. |
| Chapter 10 | Advanced algorithms were explanation-only | Added a tested Kruskal minimum-spanning-tree implementation in both languages. |
| Chapter 11 | Only the streaming-median combination was tested | Added tested LRU cache, Randomized Set, TimeMap, and Min Stack implementations in both languages. |
| Chapters 2–4 and 7–9 | At least one paired or equivalent example | Retained existing tested examples and verified coverage automatically. |

Advanced Graphs and Composite Structures now use subpages. Pages with
explanation-only blueprints say so explicitly; only snippets sourced from
`codes/` are presented as tested templates.

## Quality rules

- Chapters 1–11 must contain both Python and C++17 code.
- Included snippets must resolve to real source markers.
- Algorithm behavior is tested outside the generated documentation.
- Advanced material is labeled explanatory until its implementation has tests.
- Template comments explain invariants, ownership changes, or discard logic
  instead of restating the next line of syntax.
- New chapter changes fail validation when either language sample is absent.
