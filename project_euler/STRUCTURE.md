# Project Euler Structure

This directory stores the Project Euler section of the website as one self-contained editorial archive inside a broader personal site.

The current canonical per-problem layout is:

```text
project_euler/problem_XXX/
  solution.md
  solution.tex
  solution.cpp
  solution.py
  statement.html
```

`solution.tex` is present when a real editable LaTeX source exists. No placeholder LaTeX files should be introduced.

## Source roles

- `statement.html` is the mirrored Project Euler problem statement prepared for direct web rendering.
- `solution.tex` is the editable mathematical source when a real LaTeX source exists.
- `solution.md` is the current website rendering layer used by Astro.
- `solution.cpp` and `solution.py` are the reference implementations shown on the page.

## Required sections

Every Project Euler entry should support the following structural sections, either directly or through the shared page layout:

- Title
- Mathematical Development
- Algorithm
- Complexity Analysis
- Answer
- Code

## Optional sections

The following sections are optional and should be preserved whenever they already exist:

- Problem Statement
- Preliminaries
- Lemmas / Propositions / Theorems
- Numerical Evaluation
- Remarks / Notes

## Editorial rules

- Never remove existing sections simply to make a page shorter.
- Never simplify proofs, derivations, or the author's reasoning style.
- Preserve full mathematical rigor.
- Preserve the author's original argument and voice.
- Keep section naming and heading hierarchy as consistent as possible across the archive.

## Heading hierarchy

- Use one `#` heading for the problem title.
- Use `##` for major sections.
- Use `###` and `####` for subordinate mathematical structure such as preliminaries, lemmas, propositions, or proof subparts.
- Do not flatten heading levels just to reduce visual depth.
- The page table of contents depends on correct heading nesting.

## Navigation

- Every Project Euler page should support Previous / Next navigation.
- Problem `N` links to `N-1` and `N+1` when those entries exist in the archive.
- Problem `1` has no previous link.
- The last archived problem has no next link.

## Rendering model

- Astro currently renders the Project Euler page body from `solution.md`.
- When `statement.html` exists, it is injected above the local write-up and the duplicate `## Problem Statement` section in `solution.md` is suppressed during loading.
- The `Code` section is rendered from `solution.cpp` and `solution.py` through the shared Project Euler source component.

## Maintenance policy

- Keep the per-problem file bundle together.
- Prefer structural or template improvements over hand-editing large numbers of entries.
- If a future workflow renders directly from LaTeX, keep `solution.md` conceptually aligned until the rendering pipeline is formally changed.
