(() => {
  "use strict";

  const bookmarkKey = "dsa-atlas-reading-place-v1";

  document.documentElement.classList.add("dsa-atlas");

  function readBookmark() {
    try {
      return JSON.parse(localStorage.getItem(bookmarkKey) || "null");
    } catch {
      return null;
    }
  }

  function pageTitle() {
    const heading = document.querySelector(".md-content h1");
    return heading?.textContent?.replace("¶", "").trim() || document.title;
  }

  function nearestSection() {
    const headings = [...document.querySelectorAll(".md-content h1[id], .md-content h2[id], .md-content h3[id]")];
    const threshold = window.scrollY + 180;
    return headings.filter((heading) => heading.offsetTop <= threshold).at(-1) || headings[0];
  }

  function bookmarkLabel(bookmark) {
    if (!bookmark) {
      return { title: "No saved place yet", detail: "Save a section to continue later." };
    }
    return {
      title: bookmark.title,
      detail: bookmark.section ? `Continue at ${bookmark.section}` : "Continue reading",
    };
  }

  function renderBookmark() {
    const resume = document.querySelector(".reader-bookmark__resume");
    if (!resume) return;
    const label = bookmarkLabel(readBookmark());
    resume.replaceChildren();
    const title = document.createElement("strong");
    const detail = document.createElement("small");
    title.textContent = label.title;
    detail.textContent = label.detail;
    resume.append(title, detail);
  }

  function savePlace() {
    const section = nearestSection();
    const bookmark = {
      path: window.location.pathname,
      hash: section?.id ? `#${section.id}` : "",
      y: Math.round(window.scrollY),
      title: pageTitle(),
      section: section?.textContent?.replace("¶", "").trim() || "",
    };
    localStorage.setItem(bookmarkKey, JSON.stringify(bookmark));
    renderBookmark();

    const button = document.querySelector(".reader-bookmark__save");
    if (button) {
      button.textContent = "Place saved";
      window.setTimeout(() => {
        button.textContent = "Save current place";
      }, 1600);
    }
  }

  function resumeReading() {
    const bookmark = readBookmark();
    if (!bookmark) return;
    if (bookmark.path === window.location.pathname) {
      window.scrollTo({ top: bookmark.y || 0, behavior: "smooth" });
      return;
    }
    window.location.assign(`${bookmark.path}${bookmark.hash || ""}`);
  }

  function installRailTools() {
    const scrollwrap = document.querySelector(".md-sidebar--primary .md-sidebar__scrollwrap");
    if (!scrollwrap || scrollwrap.querySelector(".reader-rail")) return;

    const rail = document.createElement("section");
    rail.className = "reader-rail";
    rail.setAttribute("aria-label", "Book navigation and reading tools");
    rail.innerHTML = `
      <p class="reader-rail__eyebrow">Book contents</p>
      <p class="reader-rail__title">12 chapters · C++11/17 + Python 3</p>
      <a
        class="reader-companion-link"
        href="https://buicongnguyen.github.io/SystemDesign/"
      >
        <span>Companion book</span>
        <strong>System Design Atlas <span aria-hidden="true">↗</span></strong>
        <small>Systems, backend, hardware, embedded, and ACiM design</small>
      </a>
      <div class="reader-bookmark">
        <button class="reader-bookmark__save" type="button">Save current place</button>
        <button class="reader-bookmark__resume" type="button"></button>
      </div>
    `;
    scrollwrap.prepend(rail);
    rail.querySelector(".reader-bookmark__save").addEventListener("click", savePlace);
    rail.querySelector(".reader-bookmark__resume").addEventListener("click", resumeReading);
    renderBookmark();
  }

  function installPageToc() {
    const article = document.querySelector(".md-content__inner");
    const source = document.querySelector(".md-sidebar--secondary .md-nav__list");
    if (!article || !source || article.querySelector(".reader-toc")) return;

    const links = [...source.querySelectorAll("a[href^='#']")];
    if (!links.length) return;

    const toc = document.createElement("details");
    toc.className = "reader-toc";
    const summary = document.createElement("summary");
    summary.textContent = "On this page";
    const container = document.createElement("nav");
    container.className = "reader-toc__links";
    container.setAttribute("aria-label", "On this page");
    for (const link of links) {
      const clone = document.createElement("a");
      clone.href = link.getAttribute("href");
      clone.textContent = link.textContent.trim();
      container.append(clone);
    }
    toc.append(summary, container);

    const heading = article.querySelector("h1");
    if (heading) article.insertBefore(toc, heading);
  }

  function installProgress() {
    if (document.querySelector(".reader-progress")) return;
    const progress = document.createElement("div");
    progress.className = "reader-progress";
    progress.setAttribute("aria-hidden", "true");
    progress.innerHTML = '<div class="reader-progress__bar"></div>';
    document.body.append(progress);
  }

  function updateProgress() {
    const bar = document.querySelector(".reader-progress__bar");
    if (!bar) return;
    const available = document.documentElement.scrollHeight - window.innerHeight;
    const progress = available > 0 ? Math.min(1, Math.max(0, window.scrollY / available)) : 1;
    bar.style.transform = `scaleX(${progress})`;
  }

  function initializeReader() {
    installRailTools();
    installPageToc();
    installProgress();
    updateProgress();
  }

  if (!window.__dsaReaderListening) {
    window.__dsaReaderListening = true;
    window.addEventListener("scroll", updateProgress, { passive: true });
    window.addEventListener("resize", updateProgress);
  }

  if (typeof document$ !== "undefined") {
    document$.subscribe(initializeReader);
  } else if (document.readyState === "loading") {
    document.addEventListener("DOMContentLoaded", initializeReader, { once: true });
  } else {
    initializeReader();
  }
})();
