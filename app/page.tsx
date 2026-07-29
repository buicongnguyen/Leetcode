import Link from "next/link";

const chapters = [
  ["01", "Solve before coding", "Move from constraints to a defensible invariant.", "chapter_01_problem_solving"],
  ["03", "Arrays and hashing", "Replace repeated scans with remembered state.", "chapter_03_arrays_hashing"],
  ["06", "Graphs", "Choose traversal from the meaning of each edge.", "chapter_06_graphs"],
  ["09", "Dynamic programming", "Define state and evaluation order before recurrence.", "chapter_09_dynamic_programming"],
];

export default function Home() {
  return (
    <main>
      <nav aria-label="Primary navigation">
        <Link className="brand" href="/">
          <span aria-hidden="true">&lt;/&gt;</span>
          DSA Atlas
        </Link>
        <div>
          <a href="/book/">Read the book</a>
          <a href="https://buicongnguyen.github.io/SystemDesign/">
            System Design
          </a>
          <a href="https://github.com/buicongnguyen/Leetcode">GitHub</a>
        </div>
      </nav>

      <section className="hero">
        <div>
          <p className="eyebrow">LeetCode reference system</p>
          <h1>
            Turn problem signals into <em>working code.</em>
          </h1>
          <p className="lede">
            A chapter-based handbook with tested C++17 and Python 3
            implementations, built around constraints, invariants, and boundary
            cases.
          </p>
          <div className="actions">
            <a className="primary" href="/book/">
              Start reading
            </a>
            <a href="https://github.com/buicongnguyen/Leetcode/tree/main/codes">
              Browse tested code
            </a>
            <a href="https://buicongnguyen.github.io/SystemDesign/">
              Explore System Design
            </a>
          </div>
        </div>
        <pre aria-label="Problem-solving example">
          <code>{`signal      = "minimum steps"
edge_weight = 1

model       = graph(states, moves)
algorithm   = BFS
invariant   = "queue order is distance order"

prove()
implement()
test_boundaries()`}</code>
        </pre>
      </section>

      <section className="chapters" aria-labelledby="chapter-heading">
        <p className="eyebrow">The learning path</p>
        <h2 id="chapter-heading">A book you can execute.</h2>
        <div className="chapterGrid">
          {chapters.map(([number, title, description, slug]) => (
            <a key={number} href={`/book/${slug}/`}>
              <small>Chapter {number}</small>
              <strong>{title}</strong>
              <span>{description}</span>
            </a>
          ))}
        </div>
      </section>
    </main>
  );
}
