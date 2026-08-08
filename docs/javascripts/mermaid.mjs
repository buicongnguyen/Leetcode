import mermaid from "https://cdn.jsdelivr.net/npm/mermaid@11.16.0/dist/mermaid.esm.min.mjs";

mermaid.initialize({
  startOnLoad: false,
  securityLevel: "strict",
});

// Material for MkDocs reads this global when rendering Mermaid fences.
window.mermaid = mermaid;
