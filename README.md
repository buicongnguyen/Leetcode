# DSA Atlas

DSA Atlas is a chapter-based problem-solving handbook for LeetCode practice.
It pairs concise explanations with tested C++11, C++17, and Python 3
implementations.

[Read the book](https://buicongnguyen.github.io/Leetcode/) ·
[Browse the code](codes/) ·
[See the reorganization plan](REORGANIZATION_PLAN.md) ·
[Read the code review](CODE_REVIEW.md) ·
[Read the reader redesign review](BOOK_READER_REVIEW.md) ·
[Read the code-sample review](CODE_SAMPLE_REVIEW.md)

## Repository map

```text
.
├── docs/                 # Book source, one directory per chapter
├── codes/
│   ├── cpp/              # C++11-compatible headers tested in C++11 and C++17
│   └── python/           # Importable Python package
├── tests/                # Python behavior and regression tests
├── scripts/              # Book and repository validation
├── .github/workflows/    # CI and GitHub Pages
├── app/                  # Sites landing page
└── mkdocs.yml            # Book navigation and theme
```

This follows the useful separation in
[HelloAlgo](https://github.com/krahets/hello-algo): book chapters live apart
from language implementations, navigation is declared centrally, and CI checks
code instead of treating snippets as decoration. The writing and code here are
original and focused on interview problem solving.

The published book uses a persistent chapter rail, an inline page outline,
device-local reading bookmarks, and scroll progress. GitHub Pages is the
canonical public website; any workspace-restricted Sites build is only an
internal companion preview.
Advanced Graphs now has separate guides for MST, flow, negative and all-pairs
shortest paths, SCC, connectivity failures, and LCA. Composite Structures has
problem-oriented guides for ten multi-structure designs, with tested paired
templates for LRU, Randomized Set, Streaming Median, TimeMap, and Min Stack.
Dynamic Programming has a state-design guide plus dedicated 1D, 2D, 3D,
higher-dimensional, knapsack, structured-pattern, and optimization pages.

## Local development

Prerequisites: Python 3.11+, Node.js 22+, and a compiler supporting C++11.

```bash
python -m pip install -r requirements.txt
python -m mkdocs serve
```

Open the local address printed by MkDocs. To run all checks:

```bash
python -m unittest discover -s tests/python -v
python scripts/check_book.py
cmake -S codes/cpp -B build/cpp
cmake --build build/cpp
ctest --test-dir build/cpp --output-on-failure
python -m mkdocs build --strict
```

## Contributing

Add explanations under the relevant `docs/chapter_*` directory and place
executable implementations under `codes/`. Every algorithm change should add
or update a behavior test. See
[the contribution guide](docs/appendix/contributing.md).

## License

MIT. See [LICENSE](LICENSE).
