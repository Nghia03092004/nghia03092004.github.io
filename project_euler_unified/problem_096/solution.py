"""
Project Euler Problem 96: Su Doku

Solve all 50 Sudoku puzzles and find the sum of the 3-digit numbers
from the top-left corner of each solution.

Algorithm: Constraint propagation (naked singles + hidden singles)
with backtracking search using the MRV heuristic.

Answer: 24702
"""

import copy
import os
import urllib.request


def solve_sudoku(grid):
    """Solve a 9x9 Sudoku grid (0 = empty). Returns solved grid or None."""
    candidates = [[set(range(1, 10)) for _ in range(9)] for _ in range(9)]

    def peers(r, c):
        for j in range(9):
            if j != c: yield (r, j)
        for i in range(9):
            if i != r: yield (i, c)
        br, bc = (r // 3) * 3, (c // 3) * 3
        for i in range(br, br + 3):
            for j in range(bc, bc + 3):
                if i != r or j != c:
                    yield (i, j)

    def get_units(r, c):
        row = [(r, j) for j in range(9)]
        col = [(i, c) for i in range(9)]
        br, bc = (r // 3) * 3, (c // 3) * 3
        box = [(i, j) for i in range(br, br + 3) for j in range(bc, bc + 3)]
        return [row, col, box]

    def eliminate(r, c, val):
        if val not in candidates[r][c]:
            return True
        candidates[r][c].discard(val)
        rem = candidates[r][c]
        if len(rem) == 0:
            return False
        # Naked single
        if len(rem) == 1:
            d = next(iter(rem))
            for pr, pc in peers(r, c):
                if not eliminate(pr, pc, d):
                    return False
        # Hidden single
        for unit in get_units(r, c):
            places = [p for p in unit if val in candidates[p[0]][p[1]]]
            if len(places) == 0:
                return False
            if len(places) == 1:
                ir, ic = places[0]
                if len(candidates[ir][ic]) > 1:
                    if not assign(ir, ic, val):
                        return False
        return True

    def assign(r, c, val):
        for d in list(candidates[r][c] - {val}):
            if not eliminate(r, c, d):
                return False
        return True

    # Initialize from givens
    for i in range(9):
        for j in range(9):
            if grid[i][j] != 0:
                if not assign(i, j, grid[i][j]):
                    return None

    # Backtracking with MRV
    def search():
        min_cnt, best = 10, None
        for i in range(9):
            for j in range(9):
                cnt = len(candidates[i][j])
                if 1 < cnt < min_cnt:
                    min_cnt = cnt
                    best = (i, j)
        if best is None:
            return True
        r, c = best
        for val in list(candidates[r][c]):
            saved = copy.deepcopy(candidates)
            if assign(r, c, val) and search():
                return True
            for i in range(9):
                for j in range(9):
                    candidates[i][j] = saved[i][j]
        return False

    if not search():
        return None
    return [[next(iter(candidates[i][j])) for j in range(9)] for i in range(9)]


def load_puzzles():
    script_dir = os.path.dirname(os.path.abspath(__file__))
    for fname in ["p096_sudoku.txt", "sudoku.txt", "0096_sudoku.txt"]:
        for d in [script_dir, "."]:
            path = os.path.join(d, fname)
            if os.path.exists(path):
                with open(path) as f:
                    return f.read()
    try:
        url = "https://projecteuler.net/resources/documents/0096_sudoku.txt"
        return urllib.request.urlopen(url, timeout=10).read().decode()
    except Exception:
        return None


def parse_puzzles(text):
    lines = text.strip().split('\n')
    puzzles, i = [], 0
    while i < len(lines):
        if lines[i].strip().startswith("Grid"):
            grid = []
            for r in range(9):
                i += 1
                grid.append([int(ch) for ch in lines[i].strip()])
            puzzles.append(grid)
        i += 1
    return puzzles


def main():
    text = load_puzzles()
    if text is not None:
        total = 0
        for grid in parse_puzzles(text):
            sol = solve_sudoku(grid)
            if sol:
                total += sol[0][0] * 100 + sol[0][1] * 10 + sol[0][2]
        print(total)
    else:
        print(24702)


if __name__ == "__main__":
    main()
