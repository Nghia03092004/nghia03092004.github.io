# Competitive Programming Structure

This directory is organized by contest family first, then by year, then by problem folder.

## Layout

```text
competitive_programming/
  SOURCES.md
  STRUCTURE.md
  icpc/
    2024/
      contest_problems.pdf
      A-billboards/
        meta.json
        solution.tex
        solution.cpp
        statement.txt
        statement.pdf
  ioi/
    2024/
      message/
        solution.tex
        solution.cpp
```

## Design rules

- `icpc/` and `ioi/` are separate top-level archive tracks.
- Each year gets its own directory.
- Each problem gets its own directory.
- The problem directory is the place to edit and maintain that entry.
- `solution.tex` is the written-source canonical file.
- `solution.cpp` is the implementation-source canonical file.

## Why this shape

This structure is intended to stay:

- easy to browse on GitHub
- easy to turn into public pages that browse by track, year, then problem
- easy to extend with more yearly folders
- easy to extend with more contest tracks later
- easy for the website build to scan without hidden conventions

## Track-specific notes

### ICPC

ICPC problem folders preserve the existing contest-style metadata and statement assets when they are useful:

- `meta.json`
- `statement.txt`
- `statement.pdf`

Year folders may also keep `contest_problems.pdf`.

### IOI

IOI problem folders are intentionally minimal:

- `solution.tex`
- `solution.cpp`

The folder name identifies the task, while the normalized filenames keep the per-entry contract consistent across the archive.
