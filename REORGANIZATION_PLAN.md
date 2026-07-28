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

## Definition of done

- Every navigation entry resolves.
- Python tests pass.
- C++17 tests compile and pass.
- MkDocs builds in strict mode.
- GitHub Pages deploys from `main`.
- The production book and repository links are reachable.
