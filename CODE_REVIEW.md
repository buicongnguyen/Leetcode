# Logic and code review

Review scope: the pre-reorganization `main` branch at `74bf53a`, followed by a
second pass over the extracted libraries and CI at `c0cd647`, and a third pass
over the expanded DP, graph, composite-structure, and book build surfaces on
2026-07-31. A fourth compatibility pass on 2026-08-08 reviewed every paired
language sample and the complete C++ library under C++11 and C++17. A fifth
curriculum-completeness pass on 2026-08-08 reviewed the new core, advanced-tree,
advanced-graph, composite, catalog, and deployment surfaces.

## Summary

The original site had strong explanatory instincts: it emphasized constraints,
invariants, algorithm selection, accessibility, and paired languages. Its main
risk was not presentation but verification. Thirty-seven templates lived as
strings in a 1,600-line browser file and the test checked for marker text rather
than behavior.

## Findings addressed

### High — Published algorithm code was never executed

The site check counted titles and searched for strings. It could not detect
syntax errors, wrong boundaries, overflow, or C++/Python drift.

**Resolution:** implementations now live in importable/compilable source files
with behavior tests in both languages.

### High — Pages deployed the whole repository

The workflow uploaded `.`. That exposed development-only files and made
deployment depend on repository layout instead of a reproducible build.

**Resolution:** CI builds MkDocs and uploads only the generated `site/`
directory.

### Medium — Median averaging could overflow in C++

`(lower.top() + upper.top()) / 2.0` adds two `int` values before conversion.
Two large inputs can overflow.

**Resolution:** the extracted implementation converts before addition and has a
regression test using `INT_MAX`.

### Medium — Topological sort did not report cycles

Kahn's algorithm returned a partial order when a cycle existed, leaving callers
to remember a separate length check.

**Resolution:** both libraries raise/throw when the result contains fewer than
`n` vertices; tests cover a directed cycle.

### Medium — Duplicate-skipping backtracking had a hidden precondition

The adjacent-duplicate rule only works when candidates are sorted. The snippet
did not sort or state that precondition.

**Resolution:** the chapter now names the precondition and the library helper
sorts its own copy before searching.

### Low — Historical map lookup mutated missing keys in C++

Using `history[key]` during `get` inserted an empty history for unknown keys.

**Resolution:** the chapter recommends `find` for read-only lookup. This
structure remains a design pattern rather than part of the initial core
library.

### Low — DOM tests were coupled to exact prose

The check asserted exact IDs, source-domain strings, and title counts, but did
not parse links or verify generated output.

**Resolution:** `scripts/check_book.py` validates declared navigation, chapter
front matter, local links, code trees, and workflow isolation.

### High — C++ Release tests could pass without checking behavior

The executable used the standard `assert` macro while CI configured a Release
build. Toolchains commonly define `NDEBUG` for Release, compiling every
assertion out and leaving a test that only printed success.

**Resolution:** the C++ suite now uses an always-on expectation helper and
returns failure when any expectation is false.

### High — Graph inputs could trigger undefined behavior in C++

Invalid sources and edge endpoints indexed vectors without validation. Python
also accepted negative indices, making invalid graph identifiers silently refer
to vertices at the end of a list.

**Resolution:** graph entry points and disjoint sets validate sizes, sources,
and endpoints consistently in both languages; regression tests cover invalid
inputs.

### Medium — Dijkstra validated only reachable negative edges

A negative edge in a disconnected component went unnoticed, even though the
function contract requires a nonnegative graph. C++ distance addition could
also overflow for extreme weights.

**Resolution:** both implementations validate all edges before traversal, and
the C++ implementation guards distance addition.

### Medium — Binary-search boundary accepted a negative domain

`first_true(-1, predicate)` silently returned `0`, which is outside its declared
domain contract.

**Resolution:** both implementations reject negative sizes.

### High — Two-robot DP confused an invalid move with a valid score

The Python implementation used `-10**18` as an impossible-state sentinel. A
valid grid may contain smaller values, allowing an out-of-bounds move to beat
the only legal path.

**Resolution:** invalid moves now return `None`, only valid suffixes participate
in maximization, and an extreme-negative regression locks the behavior.

### High — Copying the C++ LRU cache invalidated its ownership model

The generated copy constructor copied list iterators stored in the lookup map.
Those iterators still referred to the original cache's list, so later access
could mutate the wrong instance or dereference invalid state.

**Resolution:** `LRUCache` is explicitly non-copyable and movable. Compile-time
tests enforce both sides of that contract.

### Medium — Generated deployment artifacts polluted lint input

Local audit and deployment staging directories were not ignored consistently.
ESLint could scan generated JavaScript and report failures unrelated to source.

**Resolution:** Git and ESLint now share explicit rules for audit output, C++
build variants, clean deployment trees, and packaged site archives. The tracked
`build/sites-vite-plugin.ts` source remains included.

### Medium — Sample verification existed only as prose

Advanced pages described some examples as tested and others as blueprints, but
the validator did not enforce that distinction.

**Resolution:** Chapter 9–11 pages now declare a sample status. Tested pages
must include paired Python and C++ snippets from repository sources;
conceptual pages cannot embed source snippets.

### Medium — A C++11 tab could become an unverified label

Duplicating a C++17 snippet into a tab named C++11 would look complete while
still allowing `optional`, structured bindings, class template argument
deduction, or generic lambdas to break on the older standard.

**Resolution:** the shared reference header now uses C++11-compatible syntax,
with explicit types where newer deduction features were previously used. CMake
builds and executes the complete suite separately under C++11 and C++17. The
book validator requires every paired sample to repeat the exact Python,
C++17, C++11 tab order.

### Medium — Two Sum exposed a C++17-only return type

The reference API returned `std::optional`, preventing the otherwise portable
header from compiling under C++11.

**Resolution:** the C++ API now returns the conventional LeetCode-style vector
of two indices, or an empty vector when no solution exists. Existing distinct-
index and overflow regressions cover the revised contract in both standards.

### High — Negative constructor sizes could allocate before validation

The first C++ review draft validated `Dinic`, `FenwickTree`, and
`SnapshotArray` sizes in the constructor body. Their vectors had already seen
the signed size in the initializer list, so a negative input could convert to a
huge unsigned allocation before the guard ran.

**Resolution:** validate before `resize`/`assign`; invalid sizes are rejected
without attempting allocation.

### Medium — LCA preprocessing could accept malformed “trees”

Skipping every edge to a parent allowed a root self-loop or parallel parent
edges to evade the traversal cycle check. One-way adjacency could also be
accepted even though the API promises an undirected tree.

**Resolution:** both languages now reject self-loops and duplicates, require
exactly `n - 1` undirected edges, verify reverse adjacency, and still check
connectedness and cycles during iterative preprocessing.

### Medium — Maximum-flow accumulation lacked an overflow boundary

Individual capacities used `long long`, but adding several valid blocking flows
could overflow the total even when every edge value was representable.

**Resolution:** the C++ Dinic template checks the aggregate before addition;
the Python integer remains unbounded.

### High — Diagram source rendered as blank space

The Markdown validator proved that every Mermaid fence had accessible text, but
browser review showed that the theme replaced each fence with an empty diagram
container. The runtime needed to be made explicit for this theme build.

**Resolution:** load a pinned Mermaid 11 runtime using Material's documented
integration, expose it to the theme, validate that configuration in
`check_book.py`, and visually verify diagrams on desktop and mobile. Dense
mobile diagrams remain inside a horizontal scroll region at a readable width.

## Current verification

- Python behavior suite: 31 tests pass.
- C++11 and C++17 Release builds compile from the same source and pass the
  same always-on behavior suite.
- Randomized differential review: 14,801 assertion groups across DP and
  composite structures passed against small reference models.
- Book validation covers 52 navigated pages and checks links, snippet markers, diagram
  accessibility, sample-status contracts, three-tab language order, dual C++
  test targets, the pinned diagram runtime, and Pages artifact isolation.

## Remaining follow-up

- Add randomized parity tests that run identical fixtures against C++ and
  Python.
- Replace recursive bridge search before positioning it as safe for graphs deep
  enough to exceed the Python or native call stack.
- Add tested LFU, Design Twitter, and All O(1) implementations only if their
  larger mutation surfaces receive reference-model and randomized tests.
- Consider a configurable KD-tree implementation only after the book chooses a
  dimension, distance metric, and update contract; the current page is
  intentionally conceptual.
