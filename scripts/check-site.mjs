import { readFile, access } from "node:fs/promises";

const required = ["index.html", "styles.css", "app.js", "DSA_Problem_Solving_Handbook_CPP_Python.docx"];
await Promise.all(required.map(file => access(file)));

const html = await readFile("index.html", "utf8");
const js = await readFile("app.js", "utf8");

for (const id of ["main", "flow", "selector", "templates", "checklist"]) {
  if (!html.includes(`id="${id}"`)) throw new Error(`Missing required landmark: ${id}`);
}
for (const target of ["styles.css", "app.js", "DSA_Problem_Solving_Handbook_CPP_Python.docx"]) {
  if (!html.includes(target)) throw new Error(`Missing reference to ${target}`);
}
if (!js.includes("const templates = [") || !js.includes("render();")) {
  throw new Error("Template application did not initialize correctly");
}

console.log("Static site validation passed.");
