"""Validate book topology, local links, snippets, and deployment isolation."""

from __future__ import annotations

import re
from pathlib import Path
from urllib.parse import unquote

import yaml

ROOT = Path(__file__).resolve().parents[1]
DOCS = ROOT / "docs"


class ConfigurationLoader(yaml.SafeLoader):
    """Safe loader that preserves MkDocs' named Python extension hook as text."""


ConfigurationLoader.add_multi_constructor(
    "tag:yaml.org,2002:python/name:",
    lambda _loader, suffix, _node: suffix,
)


def nav_pages(value: object) -> list[str]:
    if isinstance(value, str):
        return [value]
    if isinstance(value, list):
        pages: list[str] = []
        for item in value:
            pages.extend(nav_pages(item))
        return pages
    if isinstance(value, dict):
        pages = []
        for item in value.values():
            pages.extend(nav_pages(item))
        return pages
    return []


def markdown_target(source: Path, raw_target: str) -> Path | None:
    target = unquote(raw_target.split("#", 1)[0])
    if not target or target.startswith(("http://", "https://", "mailto:")):
        return None
    if target.startswith("/"):
        return None
    resolved = (source.parent / target).resolve()
    if target.endswith("/"):
        resolved /= "index.md"
    if resolved.suffix == "":
        markdown = resolved.with_suffix(".md")
        if markdown.exists():
            resolved = markdown
    return resolved


def main() -> None:
    configuration = yaml.load(
        (ROOT / "mkdocs.yml").read_text(encoding="utf-8"),
        Loader=ConfigurationLoader,
    )
    declared = {DOCS / page for page in nav_pages(configuration["nav"])}
    markdown_files = set(DOCS.rglob("*.md"))

    missing = sorted(path for path in declared if not path.is_file())
    if missing:
        raise SystemExit(f"Navigation references missing pages: {missing}")

    omitted = sorted(markdown_files - declared)
    if omitted:
        raise SystemExit(f"Markdown pages omitted from navigation: {omitted}")

    link_pattern = re.compile(r"(?<!!)\[[^\]]+\]\(([^)\s]+)(?:\s+[^)]*)?\)")
    snippet_pattern = re.compile(r'--8<-- "([^"]+?)(?::([^"]+))?"')
    mermaid_pattern = re.compile(r"```mermaid\s*\n(.*?)```", flags=re.DOTALL)
    language_tab_pattern = re.compile(
        r'^=== "(Python|C\+\+17|C\+\+11)"\s*$', flags=re.MULTILINE
    )

    for page in sorted(markdown_files):
        content = page.read_text(encoding="utf-8")
        if not content.startswith("---\n") or "\ndescription:" not in content.split("---", 2)[1]:
            raise SystemExit(f"{page.relative_to(ROOT)} needs description front matter")
        metadata = yaml.safe_load(content.split("---", 2)[1])
        snippets = snippet_pattern.findall(content)
        language_tabs = language_tab_pattern.findall(content)

        if language_tabs:
            expected_tabs = ["Python", "C++17", "C++11"] * (
                len(language_tabs) // 3
            )
            if language_tabs != expected_tabs:
                raise SystemExit(
                    f"{page.relative_to(ROOT)} language tabs must repeat "
                    "Python, C++17, C++11"
                )
            for language, fence in (
                ("Python", "python"),
                ("C++17", "cpp"),
                ("C++11", "cpp"),
            ):
                populated_tabs = re.findall(
                    rf'^=== "{re.escape(language)}"\s*\n\n    ```{fence}',
                    content,
                    flags=re.MULTILINE,
                )
                if len(populated_tabs) != language_tabs.count(language):
                    raise SystemExit(
                        f"{page.relative_to(ROOT)} has an empty or malformed "
                        f"{language} tab"
                    )

        if page.parent.name.startswith(
            ("chapter_09_", "chapter_10_", "chapter_11_")
        ):
            sample_status = metadata.get("sample_status")
            if sample_status not in {"tested", "conceptual"}:
                raise SystemExit(
                    f"{page.relative_to(ROOT)} needs sample_status: tested or conceptual"
                )

            python_snippet = any(path.startswith("codes/python/") for path, _ in snippets)
            cpp_snippet = any(path.startswith("codes/cpp/") for path, _ in snippets)
            visible_pair = (
                "```python" in content
                and "```cpp" in content
                and '=== "C++11"' in content
            )
            if sample_status == "tested" and not (
                python_snippet and cpp_snippet and visible_pair
            ):
                raise SystemExit(
                    f"{page.relative_to(ROOT)} is tested but lacks visible paired source snippets"
                )
            if sample_status == "conceptual" and snippets:
                raise SystemExit(
                    f"{page.relative_to(ROOT)} is conceptual but includes source snippets"
                )

        headings = re.findall(r"^# (.+)$", content, flags=re.MULTILINE)
        if len(headings) != 1:
            raise SystemExit(f"{page.relative_to(ROOT)} must contain exactly one h1")

        for raw_target in link_pattern.findall(content):
            target = markdown_target(page, raw_target)
            if target is not None and not target.exists():
                raise SystemExit(
                    f"Broken local link in {page.relative_to(ROOT)}: {raw_target}"
                )

        for snippet_path, marker in snippets:
            source = ROOT / snippet_path
            if not source.is_file():
                raise SystemExit(
                    f"Missing snippet in {page.relative_to(ROOT)}: {snippet_path}"
                )
            if marker:
                snippet_source = source.read_text(encoding="utf-8")
                start = f"[start:{marker}]"
                end = f"[end:{marker}]"
                if start not in snippet_source or end not in snippet_source:
                    raise SystemExit(
                        f"Missing snippet marker {marker!r} in {snippet_path}"
                    )

        for diagram_number, diagram in enumerate(
            mermaid_pattern.findall(content), start=1
        ):
            if "accTitle:" not in diagram or "accDescr:" not in diagram:
                raise SystemExit(
                    f"Mermaid diagram {diagram_number} in "
                    f"{page.relative_to(ROOT)} needs accTitle and accDescr"
                )

    for required in (
        ROOT / "codes/python/dsa_atlas",
        ROOT / "codes/cpp/include/dsa_atlas",
        ROOT / "tests/python",
        ROOT / "codes/cpp/tests",
        ROOT / "BOOK_READER_REVIEW.md",
        ROOT / "CODE_SAMPLE_REVIEW.md",
        ROOT / "CODE_REVIEW.md",
        ROOT / "REORGANIZATION_PLAN.md",
    ):
        if not required.exists():
            raise SystemExit(f"Required project surface is missing: {required}")

    cpp_cmake = (ROOT / "codes/cpp/CMakeLists.txt").read_text(encoding="utf-8")
    for target in ("dsa_atlas_tests_cpp11 11", "dsa_atlas_tests_cpp17 17"):
        if target not in cpp_cmake:
            raise SystemExit(f"C++ dual-standard test target is missing: {target}")

    pages_workflow = (ROOT / ".github/workflows/pages.yml").read_text(encoding="utf-8")
    if re.search(r"path:\s*\.\s*$", pages_workflow, flags=re.MULTILINE):
        raise SystemExit("Pages must upload generated output, not the repository root")
    if "path: site" not in pages_workflow:
        raise SystemExit("Pages workflow must upload the generated site directory")

    reader_css = (DOCS / "stylesheets/extra.css").read_text(encoding="utf-8")
    reader_js = (DOCS / "javascripts/extra.js").read_text(encoding="utf-8")
    for selector in (".reader-rail", ".reader-bookmark", ".reader-toc", ".reader-progress"):
        if selector not in reader_css or selector not in reader_js:
            raise SystemExit(f"Reader interface is missing required component: {selector}")

    for chapter_number in range(1, 12):
        matches = sorted(DOCS.glob(f"chapter_{chapter_number:02d}_*/index.md"))
        if len(matches) != 1:
            raise SystemExit(
                f"Expected one directory for Chapter {chapter_number}, found {matches}"
            )
        content = matches[0].read_text(encoding="utf-8")
        has_python = "```python" in content
        has_cpp = "```cpp" in content
        if not has_python or not has_cpp:
            raise SystemExit(
                f"{matches[0].relative_to(ROOT)} needs visible Python and C++ samples"
            )

    legacy = [name for name in ("index.html", "app.js", "thinking.html") if (ROOT / name).exists()]
    if legacy:
        raise SystemExit(f"Legacy flat-site files remain after migration: {legacy}")

    print(
        f"Book validation passed: {len(markdown_files)} pages, "
        f"{sum(1 for _ in (ROOT / 'codes').rglob('*') if _.is_file())} code files."
    )


if __name__ == "__main__":
    main()
