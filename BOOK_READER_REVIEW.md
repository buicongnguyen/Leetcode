# Book reader redesign plan and review

## Benchmark

The Vietnamese Hello Algo reader was reviewed as an interaction benchmark, not
as a source of content or branding. Its useful structural ideas are:

- a sticky header that keeps search and book identity available;
- an independently scrollable chapter rail with strong active-page state;
- a wide, calm reading canvas instead of a narrow documentation column;
- page-level headings available near the article;
- a drawer-based chapter view on smaller screens;
- previous and next navigation for sequential reading.

## Review findings

| Priority | Finding | Resolution |
| --- | --- | --- |
| High | The existing desktop chapter list shared the page scroll and read like generic documentation navigation. | The primary rail now fills the viewport below the header and scrolls independently. |
| High | The article was constrained by a permanently visible right-hand table of contents. | The right rail is replaced by a compact, expandable “On this page” panel above the article. |
| Medium | Navigation was a flat list, so learners could not scan phases of the book. | Chapters are grouped into Start, Foundations, Core structures, Search strategies, Advanced, and Appendix. |
| Medium | There was no way to keep a reading place. | A device-local bookmark saves the current page, nearest section, and scroll position. |
| Medium | Active chapters were indicated mostly by text color. | The active page now has a filled state and left-edge marker. |
| Low | Long pages gave no sense of reading progress. | A slim progress indicator follows the page scroll. |
| Low | The home hero became cramped inside the old documentation column. | The reading canvas is wider and responsive typography keeps the hero legible. |

## Execution plan

1. Preserve Markdown chapters and tested code as the single source of truth.
2. Reorganize only the declared navigation hierarchy; do not move stable URLs.
3. Add reader behavior as a progressive enhancement over MkDocs Material.
4. Keep the built-in mobile drawer, search, theme toggle, and keyboard behavior.
5. Validate structure, local links, JavaScript lint, Python behavior, C++ behavior,
   strict book generation, responsive rendering, and both production hosts.

## Design constraints

- Bookmarks remain on the reader's device and transmit no reading history.
- The book remains usable when JavaScript is unavailable; only bookmarking,
  inline page contents, and progress enhancement disappear.
- Existing chapter URLs remain stable for external links and search indexing.
- GitHub Pages and the Sites `/book/` route are generated from the same source.
