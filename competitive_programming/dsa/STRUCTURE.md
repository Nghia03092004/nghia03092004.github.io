# DSA Structure

This directory is the source tree for the website's **Competitive Programming / Data Structures & Algorithms** section.

## Layout

```text
competitive_programming/
  dsa/
    index.tex
    taxonomy.tex
    STRUCTURE.md
    data-structures/
      index.tex
      fenwick-tree/
        meta.json
        note.tex
        code.cpp
        figures/
    graph-algorithms/
      index.tex
      dijkstra/
        meta.json
        note.tex
        code.cpp
```

## File roles

- `index.tex` at the root is the landing-page source.
- `taxonomy.tex` is the public topic map.
- Each category folder has its own `index.tex` for the category page.
- Each note folder keeps:
  - `meta.json`: title, summary, level, tags, ordering metadata
  - `note.tex`: the written source of truth for the explanation
  - `code.cpp`: the implementation source of truth
  - `figures/`: optional SVG or image assets used by `note.tex`

## Editing workflow

If you want to change the explanation:

- edit `note.tex`

If you want to change the implementation:

- edit `code.cpp`

If you want to change the note listing, short description, tags, or level:

- edit `meta.json`

Do not rewrite the Astro page files to change note content. The site reads this directory directly at build time.

## Rendering model

The website scans `competitive_programming/dsa/` during the build.

- `note.tex` and category `index.tex` files are rendered to HTML with the site's LaTeX renderer.
- `code.cpp` is shown separately in a copyable code panel.
- raw files are exposed secondarily through the `competitive-programming-assets/dsa` routes.
- diagrams in `figures/` are served from the same raw-asset pipeline, so note pages do not need duplicates in `public/`.

## Adding a new note

1. Choose the category folder.
2. Create a new note directory named with the public slug.
3. Add:
   - `meta.json`
   - `note.tex`
   - `code.cpp`
4. Add `figures/` if the note needs diagrams.
5. Rebuild the site.

The loader assumes that a note folder is complete when those three required files exist.
