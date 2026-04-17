# Personal Website

This repository contains a personal website built with Astro. It is the main long-term working repo for a broader personal web presence: essays, technical notes, engineering logs, and a large Project Euler archive.

Project Euler is an important section of the site, but it is not the whole identity of the repository.

## Main source layout

```text
.
|- .github/workflows/deploy.yml
|- project_euler/
|  |- problem_XXX/
|  |  |- solution.md
|  |  |- solution.cpp
|  |  |- solution.py
|  |  `- statement.html        # optional mirrored Project Euler statement
|- public/
|  |- favicon.ico
|  |- favicon.svg
|  `- project-euler-media/     # optional mirrored statement media
|- src/
|  |- components/
|  |- data/
|  |  |- code/
|  |  |- essays/
|  |  `- euler-solved-counts.json
|  |- layouts/
|  |- lib/
|  |- pages/
|  |- site.config.ts
|  `- styles/
|- tools/
|  |- project-euler-latex-fallback.mjs
|  `- sync-project-euler-statements.mjs
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

Build the site for production:

```bash
npm run build
```

Run Astro's checks:

```bash
npm run check
```

## Main places to edit

General site metadata:

- `src/site.config.ts`

Key pages and layout:

- `src/pages/index.astro`
- `src/pages/about.astro`
- `src/pages/project-euler/index.astro`
- `src/pages/project-euler/[slug].astro`
- `src/layouts/BaseLayout.astro`
- `src/styles/global.css`

Essay and code content:

- `src/data/essays/`
- `src/data/code/`

Project Euler content:

- `project_euler/problem_XXX/solution.md`
- `project_euler/problem_XXX/solution.cpp`
- `project_euler/problem_XXX/solution.py`

Project Euler loading and metadata:

- `src/content.config.ts`
- `src/lib/euler.ts`
- `src/lib/euler-markdown.ts`
- `src/lib/euler-statement.ts`

## How Project Euler content works

Project Euler pages are not loaded from `src/data/project-euler/`.

The active implementation reads directly from the folder-based archive under:

```text
project_euler/problem_XXX/
```

Each problem directory contains the local solution sources:

- `solution.md`
- `solution.cpp`
- `solution.py`

It may also contain:

- `statement.html`

The custom Euler collection in `src/content.config.ts` scans `project_euler/`, renders each `solution.md`, and creates the `/project-euler/problem-XXXX/` routes.

If `statement.html` exists, the site renders that mirrored statement above the local write-up and automatically suppresses the duplicated `## Problem Statement` section from `solution.md`.

## Syncing Project Euler statements

The repository includes an optional sync script:

```bash
npm run sync:project-euler-statements -- --all
```

You can also sync one problem or a range:

```bash
npm run sync:project-euler-statements -- --problem 96
npm run sync:project-euler-statements -- --from 1 --to 100
```

### What the sync script uses

`tools/sync-project-euler-statements.mjs` works from a local Project Euler TeX archive, not from the Astro content tree alone.

It expects:

- a local `../Project Euler/` directory, or `PROJECT_EULER_BOOK_ROOT`
- built statement books under `build_statement_books/`
- `make4ht`
- Playwright installed in this repo

The script:

- builds book HTML with `make4ht`
- extracts per-problem statement fragments
- falls back to local LaTeX extraction when the book HTML is not compile-safe
- copies statement media into `public/project-euler-media/`
- writes fragments to `project_euler/problem_XXX/statement.html`

If no safe local fragment is available for a problem, the script keeps the existing `statement.html` as the fallback.

### Playwright requirement

Playwright is only needed for statement syncing. It is not required for normal editing, local page development, or a regular Astro build.

If the browser binary is missing, run:

```bash
npx playwright install chromium
```

`.tmp-playwright` is not part of the repo's normal workflow and is not required. The repo uses its own local `playwright` dependency.

### Important deployment note

Generated `statement.html` files and `public/project-euler-media/` assets are part of the site source once created. If you want mirrored statements on GitHub Pages, commit and push them like any other source file.

## Deployment

GitHub Pages deployment is handled by:

```text
.github/workflows/deploy.yml
```

Pushes to `main` trigger the workflow.

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

These are local/generated items, not long-term source files:

- `dist/`
- `.astro/`
- `node_modules/`
- `build-*.log`

They can be deleted safely. Recreate dependencies with `npm install`, and recreate build output with `npm run build`.

## Project Euler copyright

Mirrored Project Euler problem content is licensed under **CC BY-NC-SA 4.0**.

If you publish mirrored statements, preserve attribution and follow the original licensing terms:

https://projecteuler.net/copyright
