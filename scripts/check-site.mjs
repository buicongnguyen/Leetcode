import { readFile, access } from "node:fs/promises";

const required = ["index.html", "advanced.html", "thinking.html", "thinking.js", "styles.css", "app.js", "DSA_Problem_Solving_Handbook_CPP_Python.docx"];
await Promise.all(required.map(file => access(file)));

const html = await readFile("index.html", "utf8");
const advancedHtml = await readFile("advanced.html", "utf8");
const thinkingHtml = await readFile("thinking.html", "utf8");
const thinkingJs = await readFile("thinking.js", "utf8");
const js = await readFile("app.js", "utf8");
const workflow = await readFile(".github/workflows/pages.yml", "utf8");

if (!html.startsWith("<!DOCTYPE html>")) throw new Error("HTML must use the canonical uppercase doctype");

for (const id of ["main", "flow", "decision-map", "selector", "templates", "checklist"]) {
  if (!html.includes(`id="${id}"`)) throw new Error(`Missing required landmark: ${id}`);
}
for (const target of ["styles.css", "app.js", "DSA_Problem_Solving_Handbook_CPP_Python.docx"]) {
  if (!html.includes(target)) throw new Error(`Missing reference to ${target}`);
}
if (!html.includes('data-catalog="basic"') || !advancedHtml.includes('data-catalog="advanced"')) {
  throw new Error("Basic and advanced catalogs must remain separated by page mode");
}
for (const id of ["choose", "deep-flows", "path-flow", "structure-flow", "flow-lifecycle", "query-flow", "templates"]) {
  if (!advancedHtml.includes(`id="${id}"`)) throw new Error(`Missing advanced-page landmark: ${id}`);
}
for (const target of ["styles.css", "app.js", "index.html"]) {
  if (!advancedHtml.includes(target)) throw new Error(`Advanced page is missing reference to ${target}`);
}
for (const id of ["master-loop", "linear-flow", "traversal-flow", "dp-flow", "state-tree", "backtracking-tree", "combinations", "lru", "sources"]) {
  if (!thinkingHtml.includes(`id="${id}"`)) throw new Error(`Missing thinking-guide landmark: ${id}`);
}
const traversalArrowCount = (thinkingHtml.match(/class="traversal-arrow"/g) || []).length;
if (traversalArrowCount !== 2) {
  throw new Error(`Traversal flow requires two connector arrows, found ${traversalArrowCount}`);
}
for (const target of ["styles.css", "thinking.js", "index.html", "advanced.html"]) {
  if (!thinkingHtml.includes(target)) throw new Error(`Thinking guide is missing reference to ${target}`);
}
for (const source of ["crackingthecodinginterview.com", "ocw.mit.edu", "algs4.cs.princeton.edu", "leetcode.com/problems/lru-cache"]) {
  if (!thinkingHtml.includes(source)) throw new Error(`Thinking guide is missing research source: ${source}`);
}
for (const composite of ["LFU cache", "Randomized set", "Streaming median", "Time map", "All O(1)", "Number containers", "Min stack", "Snapshot array"]) {
  if (!thinkingHtml.includes(composite)) throw new Error(`Thinking guide is missing composite design: ${composite}`);
}
if (!thinkingJs.includes("theme-toggle") || !thinkingJs.includes("dsa-theme")) {
  throw new Error("Thinking guide theme control did not initialize correctly");
}
if (!js.includes("const templates = [") || !js.includes("render();")) {
  throw new Error("Template application did not initialize correctly");
}
if (!js.includes("const codeMaps = {") || !js.includes('class="code-map"')) {
  throw new Error("Every template must render a concise code explanation map");
}
const templateCount = (js.match(/title: "/g) || []).length;
if (templateCount < 35) throw new Error(`Expected at least 35 templates, found ${templateCount}`);
const codeMapCount = (js.match(/^  "[^"]+": \[$/gm) || []).length;
if (codeMapCount !== templateCount) {
  throw new Error(`Expected one code map per template: ${codeMapCount} maps for ${templateCount} templates`);
}
for (const advanced of ["Dinic maximum flow", "Red-black tree insertion", "KD-tree nearest neighbor", "Kruskal minimum spanning tree"]) {
  if (!js.includes(advanced)) throw new Error(`Missing advanced template: ${advanced}`);
}
if (!js.includes("LRU cache: hash map + doubly linked list")) {
  throw new Error("Missing composite LRU cache template");
}
for (const composite of ["Randomized set: array + hash map", "Streaming median: two heaps", "Time map: hash map + sorted history", "Min stack: value stack + minimum stack"]) {
  if (!js.includes(composite)) throw new Error(`Missing composite template: ${composite}`);
}
if (!html.includes('for="search"') || !html.includes('id="search"')) {
  throw new Error("Search control must have an explicit label association");
}
if (!advancedHtml.includes('for="search"') || !advancedHtml.includes('id="search"')) {
  throw new Error("Advanced search control must have an explicit label association");
}
if (html.match(/<div(?=[^>]*aria-label=)(?![^>]*role=)[^>]*>/)) {
  throw new Error("Generic div elements with aria-label require an explicit semantic role");
}
if (!workflow.includes("actions/setup-node@v6") || !workflow.includes("node-version: 24")) {
  throw new Error("Pages validation must use the current Node 24 action runtime");
}

console.log("Static site validation passed.");
