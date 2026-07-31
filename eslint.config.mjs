import { defineConfig, globalIgnores } from "eslint/config";
import nextVitals from "eslint-config-next/core-web-vitals";
import nextTs from "eslint-config-next/typescript";

export default defineConfig([
  ...nextVitals,
  ...nextTs,
  globalIgnores([
    ".codex/**",
    ".audit-playwright-output/**",
    ".next/**",
    ".venv/**",
    "build/cpp*/**",
    "deploy-clean-*/**",
    "dist/**",
    "node_modules/**",
    "public/book/**",
    "site/**",
  ]),
]);
