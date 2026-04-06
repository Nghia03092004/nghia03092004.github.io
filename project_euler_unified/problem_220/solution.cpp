#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 220: Heighway Dragon
 *
 * L-system: Fa -> Fa R Fb, Fb -> Fa L Fb
 * Start at origin facing up (north). Find position after 10^12 steps
 * at expansion level 50.
 *
 * Approach: Recursive decomposition with precomputed displacement tables.
 *
 * Answer: 139776,963904
 */

const int MAXN = 51;

// Directions: 0=N(+y), 1=E(+x), 2=S(-y), 3=W(-x)
int dx_dir[4] = {0, 1, 0, -1};
int dy_dir[4] = {1, 0, -1, 0};

// For each level n, symbol s (0=a, 1=b), direction d:
// disp_x[n][s][d], disp_y[n][s][d] = net displacement
// rot[n][s] = net rotation (mod 4)

long long disp_x[MAXN][2][4];
long long disp_y[MAXN][2][4];
int rot[MAXN][2];

void precompute() {
    // Base case: level 0
    for (int s = 0; s < 2; s++) {
        rot[0][s] = 0;
        for (int d = 0; d < 4; d++) {
            disp_x[0][s][d] = dx_dir[d];
            disp_y[0][s][d] = dy_dir[d];
        }
    }

    // Recursive case
    for (int n = 1; n < MAXN; n++) {
        // Fa^(n) = Fa^(n-1) R Fb^(n-1)
        // rot(n, a) = rot(n-1, a) + 1 + rot(n-1, b)  (mod 4)
        rot[n][0] = (rot[n-1][0] + 1 + rot[n-1][1]) % 4;

        // Fb^(n) = Fa^(n-1) L Fb^(n-1)
        // rot(n, b) = rot(n-1, a) - 1 + rot(n-1, b)  (mod 4)
        rot[n][1] = (rot[n-1][0] - 1 + rot[n-1][1] + 4) % 4;

        for (int d = 0; d < 4; d++) {
            // Fa^(n) starting in direction d:
            // First half: Fa^(n-1) in direction d
            long long x1 = disp_x[n-1][0][d];
            long long y1 = disp_y[n-1][0][d];
            int d_after_first_a = (d + rot[n-1][0]) % 4;
            // Turn right
            int d_second_a = (d_after_first_a + 1) % 4;
            // Second half: Fb^(n-1) in direction d_second_a
            long long x2 = disp_x[n-1][1][d_second_a];
            long long y2 = disp_y[n-1][1][d_second_a];
            disp_x[n][0][d] = x1 + x2;
            disp_y[n][0][d] = y1 + y2;

            // Fb^(n) starting in direction d:
            // First half: Fa^(n-1) in direction d
            x1 = disp_x[n-1][0][d];
            y1 = disp_y[n-1][0][d];
            int d_after_first_b = (d + rot[n-1][0]) % 4;
            // Turn left
            int d_second_b = (d_after_first_b + 3) % 4; // +3 = -1 mod 4
            // Second half: Fb^(n-1) in direction d_second_b
            x2 = disp_x[n-1][1][d_second_b];
            y2 = disp_y[n-1][1][d_second_b];
            disp_x[n][1][d] = x1 + x2;
            disp_y[n][1][d] = y1 + y2;
        }
    }
}

// Find position after k steps of symbol s at level n, starting at direction d
// Returns (x, y)
pair<long long, long long> query(int n, int s, int d, long long k) {
    if (k == 0) return {0, 0};

    if (n == 0) {
        // Single step
        return {dx_dir[d], dy_dir[d]};
    }

    long long half = 1LL << (n - 1);

    // First half is always Fa^(n-1)
    // Turn depends on symbol: R for a, L for b
    // Second half is always Fb^(n-1)

    if (k <= half) {
        // Only in first half (Fa^(n-1))
        return query(n - 1, 0, d, k);
    } else {
        // Full first half + turn + partial second half
        long long x1 = disp_x[n-1][0][d];
        long long y1 = disp_y[n-1][0][d];
        int d_after = (d + rot[n-1][0]) % 4;
        int turn = (s == 0) ? 1 : 3; // R=+1 for a, L=-1=+3 for b
        int d_second = (d_after + turn) % 4;

        auto [x2, y2] = query(n - 1, 1, d_second, k - half);
        return {x1 + x2, y1 + y2};
    }
}

int main() {
    precompute();

    long long steps = 1000000000000LL; // 10^12
    int level = 50;

    // Start at origin, facing north (direction 0), symbol Fa at level 50
    auto [x, y] = query(level, 0, 0, steps);

    cout << x << "," << y << endl;
    return 0;
}
