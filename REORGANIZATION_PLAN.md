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
