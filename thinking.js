const root = document.documentElement;
const themeButton = document.querySelector("#theme-toggle");
let savedTheme = null;

try {
  savedTheme = localStorage.getItem("dsa-theme");
} catch {
  // Theme switching still works without persistent browser storage.
}

root.dataset.theme = savedTheme
  || (window.matchMedia("(prefers-color-scheme: light)").matches ? "light" : "dark");

function updateThemeControl() {
  const nextTheme = root.dataset.theme === "light" ? "dark" : "light";
  themeButton.setAttribute("aria-label", `Switch to ${nextTheme} theme`);
  themeButton.setAttribute("title", `Switch to ${nextTheme} theme`);
  document.querySelector('meta[name="theme-color"]')
    .setAttribute("content", root.dataset.theme === "light" ? "#f5f8fc" : "#07111f");
}

updateThemeControl();
themeButton.addEventListener("click", () => {
  root.dataset.theme = root.dataset.theme === "light" ? "dark" : "light";
  try {
    localStorage.setItem("dsa-theme", root.dataset.theme);
  } catch {
    // Ignore unavailable storage.
  }
  updateThemeControl();
});
