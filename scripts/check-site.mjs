import { readFile, access } from "node:fs/promises";

const required = ["index.html", "styles.css", "app.js", "DSA_Problem_Solving_Handbook_CPP_Python.docx"];
await Promise.all(required.map(file => access(file)));

const html = await readFile("index.html", "utf8");
const js = await readFile("app.js", "utf8");
const workflow = await readFile(".github/workflows/pages.yml", "utf8");

if (!html.startsWith("<!DOCTYPE html>")) throw new Error("HTML must use the canonical uppercase doctype");

for (const id of ["main", "flow", "selector", "templates", "checklist"]) {
  if (!html.includes(`id="${id}"`)) throw new Error(`Missing required landmark: ${id}`);
}
for (const target of ["styles.css", "app.js", "DSA_Problem_Solving_Handbook_CPP_Python.docx"]) {
  if (!html.includes(target)) throw new Error(`Missing reference to ${target}`);
}
if (!js.includes("const templates = [") || !js.includes("render();")) {
  throw new Error("Template application did not initialize correctly");
}
const templateCount = (js.match(/title: "/g) || []).length;
if (templateCount < 20) throw new Error(`Expected at least 20 templates, found ${templateCount}`);
if (!html.includes('for="search"') || !html.includes('id="search"')) {
  throw new Error("Search control must have an explicit label association");
}
if (html.match(/<div(?=[^>]*aria-label=)(?![^>]*role=)[^>]*>/)) {
  throw new Error("Generic div elements with aria-label require an explicit semantic role");
}
if (!workflow.includes("actions/setup-node@v6") || !workflow.includes("node-version: 24")) {
  throw new Error("Pages validation must use the current Node 24 action runtime");
}

console.log("Static site validation passed.");
