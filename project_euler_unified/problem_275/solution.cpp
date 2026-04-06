#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 275: Balanced Sculptures
 *
 * Order n = n blocks + 1 plinth = n+1 tiles.
 * Plinth at (0,0). Blocks at y >= 1. All tiles connected.
 * x_sum of blocks = 0. Reflections identified.
 *
 * Redelmeier with excluded[] array for permanently skipped cells.
 * Optimized: use bool arrays instead of set for untried membership.
 */

const int ORDER_VAL = 18;
int ORDER;
const int MAXC = 20;

const int dx[] = {0, 0, -1, 1};
const int dy[] = {-1, 1, 0, 0};

const int W = 2 * MAXC + 1; // 41
const int H = MAXC + 1;     // 21
inline int encode(int x, int y) { return y * W + (x + MAXC); }
inline int dec_x(int c) { return c % W - MAXC; }
inline int dec_y(int c) { return c / W; }

const int MAXCELLS = H * W; // 861
bool placed[MAXCELLS];
bool excluded[MAXCELLS];
bool in_untried[MAXCELLS];

long long result_total = 0;
long long result_sym = 0;

int cells_arr[25];
int n_placed;
int x_sum;

// Use a fixed-size array for untried, sorted
void dfs(int* untried, int untried_sz) {
    if (n_placed == ORDER) {
        if (x_sum == 0) {
            result_total++;
            bool sym = true;
            for (int i = 0; i < ORDER; i++) {
                int x = dec_x(cells_arr[i]), y = dec_y(cells_arr[i]);
                int mirror = encode(-x, y);
                if (!placed[mirror]) { sym = false; break; }
            }
            if (sym) result_sym++;
        }
        return;
    }

    int remaining = ORDER - n_placed;
    if (abs(x_sum) > remaining * MAXC) return;
    if (untried_sz == 0) return;

    int c = untried[0];
    int cx = dec_x(c), cy = dec_y(c);

    // Branch 1: Add c
    {
        placed[c] = true;
        in_untried[c] = false;
        cells_arr[n_placed] = c;
        n_placed++;
        x_sum += cx;

        // Find new neighbors
        int added[4];
        int n_added = 0;
        for (int d = 0; d < 4; d++) {
            int nx = cx + dx[d], ny = cy + dy[d];
            if (ny < 1 || ny > MAXC || nx < -MAXC || nx > MAXC) continue;
            int nc = encode(nx, ny);
            if (!placed[nc] && !excluded[nc] && !in_untried[nc]) {
                in_untried[nc] = true;
                added[n_added++] = nc;
            }
        }

        // Build new untried = (old - c) merged with added
        // Both old untried (from index 1) and added are sorted (added needs sorting)
        sort(added, added + n_added);
        int new_untried[100]; // should be enough
        int new_sz = 0;
        int ai = 0;
        for (int i = 1; i < untried_sz; i++) {
            while (ai < n_added && added[ai] < untried[i]) {
                new_untried[new_sz++] = added[ai++];
            }
            new_untried[new_sz++] = untried[i];
        }
        while (ai < n_added) {
            new_untried[new_sz++] = added[ai++];
        }

        dfs(new_untried, new_sz);

        // Undo
        n_placed--;
        x_sum -= cx;
        placed[c] = false;
        for (int i = 0; i < n_added; i++) in_untried[added[i]] = false;
    }

    // Branch 2: Skip c
    {
        excluded[c] = true;
        in_untried[c] = false;
        dfs(untried + 1, untried_sz - 1);
        excluded[c] = false;
        in_untried[c] = true;
    }
}

int main(int argc, char* argv[]) {
    ORDER = (argc > 1) ? atoi(argv[1]) : ORDER_VAL;
    memset(placed, false, sizeof(placed));
    memset(excluded, false, sizeof(excluded));
    memset(in_untried, false, sizeof(in_untried));

    // Place plinth at (0, 0)
    placed[encode(0, 0)] = true;
    n_placed = 0;
    x_sum = 0;

    // Initial untried: y >= 1 neighbors of (0, 0) = just (0, 1)
    int init_untried[4];
    int init_sz = 0;
    for (int d = 0; d < 4; d++) {
        int nx = dx[d], ny = dy[d];
        if (ny < 1 || ny > MAXC || nx < -MAXC || nx > MAXC) continue;
        int nc = encode(nx, ny);
        init_untried[init_sz++] = nc;
        in_untried[nc] = true;
    }
    sort(init_untried, init_untried + init_sz);

    dfs(init_untried, init_sz);

    long long answer = (result_total + result_sym) / 2;
    if (argc > 1) {
        printf("ORDER=%d: total=%lld sym=%lld answer=%lld\n",
               ORDER, result_total, result_sym, answer);
    } else {
        printf("%lld\n", answer);
    }
    return 0;
}
