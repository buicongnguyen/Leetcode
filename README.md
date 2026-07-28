# DSA Atlas

DSA Atlas is a chapter-based problem-solving handbook for LeetCode practice.
It pairs concise explanations with tested C++17 and Python 3 implementations.

[Read the book](https://buicongnguyen.github.io/Leetcode/) ·
[Browse the code](codes/) ·
[See the reorganization plan](REORGANIZATION_PLAN.md) ·
[Read the code review](CODE_REVIEW.md) ·
[Read the reader redesign review](BOOK_READER_REVIEW.md)

## Repository map

```text
.
├── docs/                 # Book source, one directory per chapter
├── codes/
│   ├── cpp/              # Reusable C++17 headers and executable tests
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
device-local reading bookmarks, and scroll progress. The same generated book is
served by GitHub Pages and by the Sites companion under `/book/`.

## Local development

Prerequisites: Python 3.11+, Node.js 22+, and a C++17 compiler.

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
