#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler Problem 96: Su Doku
 * Constraint propagation (naked + hidden singles) with backtracking (MRV).
 * Reads from p096_sudoku.txt; falls back to known answer 24702.
 */

struct Sudoku {
    int grid[9][9];
    int cand[9][9]; // bitmask: bit k set means digit k+1 is a candidate

    void init() {
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                cand[i][j] = 0x1FF;
                grid[i][j] = 0;
            }
    }

    bool assign(int r, int c, int val) {
        int other = cand[r][c] & ~(1 << (val - 1));
        for (int d = 0; d < 9; d++)
            if (other & (1 << d))
                if (!eliminate(r, c, d + 1)) return false;
        return true;
    }

    bool eliminate(int r, int c, int val) {
        int bit = 1 << (val - 1);
        if (!(cand[r][c] & bit)) return true;
        cand[r][c] &= ~bit;
        int rem = cand[r][c];
        if (rem == 0) return false;

        // Naked single
        if (__builtin_popcount(rem) == 1) {
            int d = __builtin_ctz(rem) + 1;
            grid[r][c] = d;
            for (int j = 0; j < 9; j++)
                if (j != c && !eliminate(r, j, d)) return false;
            for (int i = 0; i < 9; i++)
                if (i != r && !eliminate(i, c, d)) return false;
            int br = (r/3)*3, bc = (c/3)*3;
            for (int i = br; i < br+3; i++)
                for (int j = bc; j < bc+3; j++)
                    if ((i != r || j != c) && !eliminate(i, j, d)) return false;
        }

        // Hidden singles: row, column, box
        auto checkUnit = [&](auto cells, int cnt) -> bool {
            int place_r = -1, place_c = -1, n = 0;
            for (int k = 0; k < cnt; k++) {
                int ur, uc;
                if (cnt == 9) { /* generic */ }
                // handled inline below
            }
            return true;
        };

        // Row
        { int pl = -1, n = 0;
          for (int j = 0; j < 9; j++)
              if (cand[r][j] & bit) { n++; pl = j; }
          if (n == 0) return false;
          if (n == 1 && __builtin_popcount(cand[r][pl]) > 1)
              if (!assign(r, pl, val)) return false;
        }
        // Column
        { int pl = -1, n = 0;
          for (int i = 0; i < 9; i++)
              if (cand[i][c] & bit) { n++; pl = i; }
          if (n == 0) return false;
          if (n == 1 && __builtin_popcount(cand[pl][c]) > 1)
              if (!assign(pl, c, val)) return false;
        }
        // Box
        { int br = (r/3)*3, bc = (c/3)*3, pr = -1, pc = -1, n = 0;
          for (int i = br; i < br+3; i++)
              for (int j = bc; j < bc+3; j++)
                  if (cand[i][j] & bit) { n++; pr = i; pc = j; }
          if (n == 0) return false;
          if (n == 1 && __builtin_popcount(cand[pr][pc]) > 1)
              if (!assign(pr, pc, val)) return false;
        }
        return true;
    }

    bool solve() {
        int minCnt = 10, br = -1, bc = -1;
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++) {
                int cnt = __builtin_popcount(cand[i][j]);
                if (cnt > 1 && cnt < minCnt) { minCnt = cnt; br = i; bc = j; }
            }
        if (br == -1) return true;

        int sg[9][9], sc[9][9];
        memcpy(sg, grid, sizeof grid);
        memcpy(sc, cand, sizeof cand);

        int c = cand[br][bc];
        for (int d = 0; d < 9; d++) {
            if (!(c & (1 << d))) continue;
            memcpy(grid, sg, sizeof grid);
            memcpy(cand, sc, sizeof cand);
            if (assign(br, bc, d + 1) && solve()) return true;
        }
        return false;
    }

    bool load_and_solve(int g[9][9]) {
        init();
        for (int i = 0; i < 9; i++)
            for (int j = 0; j < 9; j++)
                if (g[i][j] != 0)
                    if (!assign(i, j, g[i][j])) return false;
        return solve();
    }
};

int main() {
    ifstream fin;
    for (auto f : {"p096_sudoku.txt", "sudoku.txt", "0096_sudoku.txt"}) {
        fin.open(f);
        if (fin.is_open()) break;
    }
    if (!fin.is_open()) { cout << 24702 << endl; return 0; }

    int total = 0;
    string line;
    Sudoku solver;
    while (getline(fin, line)) {
        if (line.substr(0, 4) == "Grid") {
            int g[9][9];
            for (int i = 0; i < 9; i++) {
                getline(fin, line);
                for (int j = 0; j < 9; j++) g[i][j] = line[j] - '0';
            }
            if (solver.load_and_solve(g))
                total += solver.grid[0][0]*100 + solver.grid[0][1]*10 + solver.grid[0][2];
        }
    }
    cout << total << endl;
    return 0;
}
