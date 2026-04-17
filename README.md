# Personal Website

This repository is the main long-term working copy of a personal website built with Astro.

The site is intended to grow into a broader personal web presence with parallel major areas such as essays, code notes, mathematics, research, and Project Euler. Project Euler is currently the largest subsystem, but it is treated as one major website section rather than the whole identity of the repository.

## Repository layout

```text
.
|- .github/workflows/deploy.yml
|- project_euler/
|  |- problem_XXX/
|  |  |- solution.md
|  |  |- solution.tex
|  |  |- solution.cpp
|  |  |- solution.py
|  |  `- statement.html
|  |- metadata.json
|  |- SOURCES.md
|  |- STRUCTURE.md
|  |- TAXONOMY.md
|  `- taxonomy.json
|- public/
|  `- project-euler-media/
|- src/
|  |- components/
|  |- data/
|  |  |- code/
|  |  `- essays/
|  |- features/
|  |  `- project-euler/
|  |     |- components/
|  |     |- data/
|  |     `- lib/
|  |- layouts/
|  |- lib/
|  |- pages/
|  |- site.config.ts
|  `- styles/
|- tools/
|  `- project-euler/
|- astro.config.mjs
|- package.json
|- package-lock.json
`- tsconfig.json
```

## Local development

Install dependencies:

```bash
npm install
```

Start the local development server:

```bash
npm run dev
```

Build for production:

```bash
npm run build
```

Run Astro checks:

```bash
npm run check
```

## Main places to edit

General site metadata:

- `src/site.config.ts`

Shared layout and styling:

- `src/layouts/BaseLayout.astro`
- `src/layouts/PostLayout.astro`
- `src/styles/global.css`

Main routes:

- `src/pages/index.astro`
- `src/pages/about.astro`
- `src/pages/project-euler/index.astro`
- `src/pages/project-euler/[slug].astro`

Essay and code content:

- `src/data/essays/`
- `src/data/code/`

Project Euler feature logic:

- `src/features/project-euler/lib/euler.ts`
- `src/features/project-euler/lib/euler-markdown.ts`
- `src/features/project-euler/lib/euler-statement.ts`
- `src/features/project-euler/lib/euler-metadata.ts`
- `src/features/project-euler/components/`

Project Euler source content:

- `project_euler/problem_XXX/solution.md`
- `project_euler/problem_XXX/solution.tex`
- `project_euler/problem_XXX/solution.cpp`
- `project_euler/problem_XXX/solution.py`
- `project_euler/problem_XXX/statement.html`

Project Euler documentation:

- `project_euler/STRUCTURE.md`
- `project_euler/SOURCES.md`
- `project_euler/TAXONOMY.md`
- `project_euler/metadata.json`

## How Project Euler works in the current site

Project Euler pages are not loaded from `src/data/project-euler/`.

The active site implementation reads directly from the folder-based archive under:

```text
project_euler/problem_XXX/
```

`src/content.config.ts` scans those problem folders, renders each `solution.md`, and builds the `/project-euler/problem-XXXX/` routes.

The current rendering model is:

- `solution.md` is the active Astro-rendered page body.
- `statement.html` is the mirrored official Project Euler statement when available.
- `solution.cpp` and `solution.py` are rendered in the shared code section.
- `solution.tex` is preserved as the editable mathematical source when a real LaTeX source exists.

If `statement.html` exists, the site injects it above the local write-up and suppresses the duplicated `## Problem Statement` section from `solution.md`.

## Project Euler source model

The Project Euler subsystem is documented inside the archive itself:

- `project_euler/STRUCTURE.md` explains editorial structure expectations.
- `project_euler/SOURCES.md` explains where the current sources came from and how they relate.
- `project_euler/TAXONOMY.md` explains the controlled tag system.
- `project_euler/metadata.json` stores per-problem tags.

The current source policy is conservative:

- do not rewrite mathematical content during structural maintenance
- preserve rigorous presentation
- keep Markdown, LaTeX, and code artifacts aligned in purpose

## Project Euler maintenance scripts

Restore canonical LaTeX sources from an external Project Euler archive:

```bash
npm run sync:project-euler-latex -- --from "C:\\path\\to\\project_euler_unified"
```

Generate controlled taxonomy metadata:

```bash
npm run generate:project-euler-metadata
```

Sync mirrored Project Euler statements:

```bash
npm run sync:project-euler-statements -- --all
```

You can also sync a single problem or a range:

```bash
npm run sync:project-euler-statements -- --problem 96
npm run sync:project-euler-statements -- --from 1 --to 100
```

## Statement sync workflow

The statement sync tooling lives under `tools/project-euler/`.

`sync-statements.mjs` expects:

- a local Project Euler book workspace at `../Project Euler/`, or `PROJECT_EULER_BOOK_ROOT`
- built statement books under `build_statement_books/`
- `make4ht`
- Playwright installed in this repository

The statement sync pipeline:

- builds book HTML with `make4ht`
- extracts per-problem statement fragments
- falls back to local LaTeX extraction when the book HTML is not compile-safe
- copies statement media into `public/project-euler-media/`
- writes fragments to `project_euler/problem_XXX/statement.html`

If no safe local fragment is available for a problem, the existing `statement.html` is preserved.

### Playwright

Playwright is optional for normal website work.

It is required only for the statement-sync workflow. It is not required for normal editing, `npm run dev`, or `npm run build`.

If the browser binary is missing, run:

```bash
npx playwright install chromium
```

`.tmp-playwright` is not part of the repository’s core structure and is not required by the current repo layout.

## Deployment

GitHub Pages deployment is handled by `.github/workflows/deploy.yml`.

Pushes to `main` trigger deployment.

The workflow supports both:

- a root site repository named `username.github.io`
- a standard repository deployed under `https://username.github.io/<repo>/`

For this repository, when the repo name is `nghia03092004.github.io`, the workflow builds with:

- `SITE_URL=https://nghia03092004.github.io`
- `BASE_PATH=/`

If the repository name changes, the workflow keeps the same site URL and switches `BASE_PATH` to `/<repo-name>`.

If you later move to a custom domain, add:

```text
public/CNAME
```

## Generated and removable local items

These are local or generated items rather than long-term source files:

- `dist/`
- `.astro/`
- `node_modules/`
- `.cache/`
- `build-*.log`

They can be removed safely and recreated as needed.

## Project Euler copyright

Mirrored Project Euler problem content is licensed under **CC BY-NC-SA 4.0**.

If you publish mirrored statements, preserve attribution and follow the original licensing terms:

https://projecteuler.net/copyright
