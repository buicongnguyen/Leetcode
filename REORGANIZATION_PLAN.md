# DSA Atlas reorganization plan

## Goal

Turn a flat static reference into a maintainable open book: chapters for
learning, runnable libraries for practice, CI for trust, and isolated build
artifacts for GitHub Pages and Sites.

## Reference architecture

HelloAlgo succeeds because it keeps three concerns distinct:

1. book source is organized by chapter;
2. implementations are organized by language;
3. automation validates languages and builds the publication.

DSA Atlas adopts those boundaries without copying HelloAlgo's content, assets,
or visual identity.

## Old-to-new map

| Previous surface | New home |
| --- | --- |
| `index.html` solve flow and basic templates | Chapters 1–9 |
| `thinking.html` decision playbook | Chapters 1, 4, 7, 8, and 9 |
| `advanced.html` advanced algorithms | Chapters 10 and 11 |
| Algorithm strings in `app.js` | `codes/cpp` and `codes/python` |
| CSS-driven navigation | `mkdocs.yml` book navigation |
| Marker-only site check | Behavior tests, compilation, strict docs build |
| Whole-repository Pages upload | Generated `site/` artifact only |
| Word snapshot | Versioned web book that can be exported later |

## Execution phases

### Phase 1 — Establish boundaries

- Create chapter directories and central navigation.
- Move algorithm implementations into language-owned source trees.
- Preserve the useful solve flow, invariants, complexity guidance, and advanced
  selection material in chapter form.

### Phase 2 — Make code trustworthy

- Add Python unit tests and a C++ executable test suite.
- Correct review findings while extracting code.
- Add a repository validator for navigation, links, and required chapter
  metadata.

### Phase 3 — Publish safely

- Build the book with MkDocs Material.
- Deploy only generated output to GitHub Pages.
- Keep the Sites landing page and `/book/` output generated from the same book
  source.

### Phase 4 — Grow deliberately

- Add a problem index with LeetCode links and difficulty metadata.
- Expand property-based and cross-language parity tests.
- Add diagrams only where they clarify state transitions or invariants.
- Produce PDF/EPUB releases from the Markdown source after the web edition
  stabilizes.

### Phase 5 — Make the book feel like a book

- Group chapters into visible learning phases without changing stable URLs.
- Keep chapter navigation fixed and independently scrollable on desktop.
- Move the per-page outline into the reading canvas.
- Add device-local bookmarks and reading progress as progressive enhancements.
- Preserve the built-in mobile chapter drawer and accessible navigation.

## 2026-07-31 stabilization plan

The next release executes four ordered gates:

1. **Build hygiene:** isolate generated C++, audit, and deployment artifacts
   from Git and lint without hiding the tracked Sites build plugin.
2. **Logic and API safety:** review the expanded DP and composite structures,
   add regressions for confirmed defects, and rebuild both language suites.
3. **Book trust contract:** classify advanced pages as `tested` or
   `conceptual`, enforce paired repository snippets on tested pages, and add a
   diagram where cross-structure ownership is easier to understand visually.
4. **Release:** run strict book, web, lint, Python, and C++ gates; commit one
   reviewed tree; push it; then verify GitHub CI, GitHub Pages, and the Sites
   deployment from that exact commit.

### Logic review of the phase order

- Hygiene comes first because generated output can create false lint findings
  and obscure the actual source diff.
- Implementations and tests come before documentation claims, so the book never
  labels an unverified blueprint as copy-ready code.
- The sample contract is checked before publication, making content drift a CI
  failure instead of a manual review convention.
- Both hosts receive the same committed source only after all local gates pass;
  deployment is not allowed to create a second, unreviewed version.
- A phase advances only after its focused logic review and regression checks
  pass. Confirmed defects are recorded in `CODE_REVIEW.md`.

## Definition of done

- Every navigation entry resolves.
- Python tests pass.
- C++11 and C++17 tests compile and pass from the same reference source.
- MkDocs builds in strict mode.
- GitHub Pages deploys from `main`.
- The production book and repository links are reachable.

## 2026-08-08 C++11 compatibility plan

1. Inventory every real paired language group rather than counting unrelated
   standalone code fences. The audit found 32 Python/C++17 groups on 24 pages.
2. Remove C++17-only dependencies from tested templates before changing the
   book UI, then run the complete behavior suite under both standards.
3. Add C++11 as the third tab in every paired group. Use explicit C++11 forms
   for conceptual examples that intentionally demonstrate newer C++17 syntax.
4. Make the validator enforce the repeating Python, C++17, C++11 order and the
   presence of both C++ test targets.
5. Run strict documentation, application, lint, Python, C++11, and C++17 gates
   before publishing.

### Logic review

- Compatibility is proved at the compiler boundary, not inferred from visual
  similarity between tabs.
- One shared tested header prevents C++11 and C++17 implementations from
  drifting apart while still letting conceptual examples teach their syntax
  differences explicitly.
- The unrelated Chapter 2 Python amortization and C++ numeric-safety examples
  remain standalone because presenting them as equivalent language tabs would
  be misleading.
- Validation checks structure as well as totals, so 32 tabs on the wrong pages
  cannot satisfy the rule accidentally.

## 2026-08-08 curriculum-completeness release

### Verified baseline

- Local `main`, `origin/main`, and the public repository all pointed to
  `eb50ec1` before this release began.
- GitHub's CI and Pages workflows both completed successfully for that commit.
- `https://buicongnguyen.github.io/Leetcode/` returned HTTP 200 from the
  canonical project URL.
- The remaining review backlog named advanced-graph templates and a structured
  problem catalog; the chapter audit also found missing linked-list, greedy,
  bit, string, trie, range-query, red-black-tree, and KD-tree coverage.

### Detailed execution plan

1. **Preserve the trust boundary.** Add executable source before changing any
   page from `conceptual` to `tested`. Exercise every new API in Python and in
   the one shared C++ header compiled under C++11 and C++17.
2. **Close core curriculum gaps.** Add Chapter 12 with linked-list reversal,
   interval scheduling, bit-counting, and KMP templates. Each lesson begins
   with recognition signals, states the invariant, uses purposeful line
   comments, and ends with failure checks and a short practice ladder.
3. **Give tree topics their own home.** Add Chapter 13 with a chooser, tested
   Trie/Fenwick/segment-tree templates, and detailed conceptual lessons for
   red-black trees and KD trees. This avoids misclassifying tree structures as
   advanced graph algorithms.
4. **Finish the graph implementation gap.** Promote Bellman–Ford,
   Floyd–Warshall, Kosaraju SCC, binary-lifting LCA, and Dinic from blueprints
   to tested templates. Add a grid-focused A* lesson so heuristic search has a
   precise, testable contract.
5. **Deepen composite reasoning.** Promote Number Containers and Snapshot
   Array as tested examples of bidirectional indexes and sparse version
   history. Keep LFU, Design Twitter, and All O(1) conceptual, but strengthen
   their ownership and operation visuals instead of presenting unverified
   large implementations.
6. **Build a practice path.** Add four-rung ladders to Chapters 3–8 and a
   searchable catalog organized by signal, pattern, difficulty, and chapter.
7. **Review and release.** Run source-marker validation, the full Python suite,
   C++11 and C++17 Release tests, strict MkDocs, web build, and lint. Review the
   generated navigation and diagrams, commit only intentional source, push
   `main`, then verify both workflows and the live Pages commit.

### Review of the plan

- **Scope:** The release closes foundational omissions and the explicitly
  recorded graph/catalog backlog. Cross-language randomized parity and an
  iterative bridge implementation remain separate follow-ups because neither
  is required to publish the new templates honestly.
- **No duplication:** Chapter 10 owns graph optimization/connectivity;
  Chapter 11 owns multi-structure design; Chapter 12 owns missing interview
  primitives; Chapter 13 owns ordered, prefix, range, and spatial trees.
- **Testability:** “Tested” means repository-owned snippets plus behavior tests
  in Python, C++11, and C++17. Pages without those artifacts must say
  `conceptual` and cannot embed source snippets.
- **Compatibility:** The C++ reference remains one C++11-compatible header.
  C++17 receives the same implementation, preventing language-tab drift.
- **Pedagogy:** Each new lesson follows recognition → model/invariant → visual
  trace → template → complexity → failure checks → practice. Diagrams are used
  only when state ownership or transitions are clearer visually.
- **Deployment:** GitHub Pages remains the only canonical public book. The
  release follows the repository's established direct-to-`main` Pages flow;
  no alternate `chatgpt.site` URL is introduced.

### Acceptance criteria

- Chapters 12 and 13, A*, and the problem catalog appear in navigation and
  pass strict link/front-matter/diagram validation.
- All promoted templates carry concise purpose comments and pass Python,
  C++11, and C++17 behavior tests.
- Every conceptual/tested label matches the snippets actually present.
- CI and Pages succeed for the pushed commit, and the canonical live URL serves
  that commit's book.

### Local execution result

- Added 15 tested templates across core, tree/range, advanced-graph, and
  composite families, with the same C++ source compiled under C++11 and C++17.
- Expanded the book to 52 navigated pages and added a 40-problem structured
  catalog plus practice ladders for Chapters 3–8.
- Passed 31 Python behavior tests, both C++ Release suites, strict book
  validation/build, ESLint, and the production web build.
- Browser review verified rendered flowcharts, three-language tabs, Chapter
  12–13 navigation, the 14-chapter rail label, no page-level horizontal
  overflow, mobile layout, and the continuous desktop rail on a long page.
