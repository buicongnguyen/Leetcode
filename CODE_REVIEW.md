# Logic and code review

Review scope: the pre-reorganization `main` branch at `74bf53a`.

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

## Remaining follow-up

- Add randomized parity tests that run identical fixtures against C++ and
  Python.
- Add complete tested implementations for Dinic, SCC, LCA, and A* before
  presenting them as copy-ready library code.
- Add a structured problem catalog instead of embedding problem lists in prose.
