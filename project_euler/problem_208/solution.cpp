#include <bits/stdc++.h>
using namespace std;

// State: (heading d, c0, c1, c2, c3) where ci = remaining visits for heading i
// c4 = remaining - c0 - c1 - c2 - c3 (but we track all 5)
// Each heading gets exactly 14 visits. Total steps = 70.

// We encode state as (d, c0, c1, c2, c3) since c4 = total_remaining - c0 - c1 - c2 - c3
// But it's easier to track all 5 counts.

// State: d * 15^4 + c0 * 15^3 + c1 * 15^2 + c2 * 15 + c3
// c4 is deduced from remaining steps = c0+c1+c2+c3+c4 and total = 70

// Actually let's use a map or a flat array.
// 5 * 15^4 = 253125 states (small enough for array)
// But we need c4 too. With c4 deducible: remaining = 70 - steps_taken
// c4 = remaining - c0 - c1 - c2 - c3... no, c0..c4 ARE the remaining counts.
// So c4 = (total remaining for heading 4).
// We don't know c4 from just c0..c3 unless we track steps taken.
// Actually: initially c0=c1=c2=c3=c4=14. As we take steps, ci decreases.
// c4 is independent of c0..c3. So we need all 5.
//
// State: (d, c0, c1, c2, c3, c4) -> 5 * 15^5 = 3,796,875 states. Still fine.
//
// Or we note that at each step from heading d, cd decreases by 1.
// So we need cd > 0 to take a step.

// Let's use memoization with a map for simplicity, or pack the state into an integer.
// Pack: d + 5*(c0 + 15*(c1 + 15*(c2 + 15*(c3 + 15*c4))))
// Max index: 5 * 15^5 = 3796875

long long dp[5 * 15 * 15 * 15 * 15 * 15]; // ~30 MB, feasible
bool visited[5 * 15 * 15 * 15 * 15 * 15];

inline int encode(int d, int c0, int c1, int c2, int c3, int c4) {
    return d + 5*(c0 + 15*(c1 + 15*(c2 + 15*(c3 + 15*c4))));
}

long long solve(int d, int c0, int c1, int c2, int c3, int c4) {
    int remaining = c0 + c1 + c2 + c3 + c4;
    if (remaining == 0) {
        return (d == 0) ? 1 : 0;
    }

    int idx = encode(d, c0, c1, c2, c3, c4);
    if (visited[idx]) return dp[idx];
    visited[idx] = true;

    int c[5] = {c0, c1, c2, c3, c4};
    long long result = 0;

    if (c[d] > 0) {
        c[d]--;
        // Left: heading becomes (d+1) % 5
        int nd = (d + 1) % 5;
        result += solve(nd, c[0], c[1], c[2], c[3], c[4]);
        // Right: heading becomes (d-1+5) % 5
        nd = (d + 4) % 5;
        result += solve(nd, c[0], c[1], c[2], c[3], c[4]);
        c[d]++;
    }

    dp[idx] = result;
    return result;
}

int main() {
    memset(visited, false, sizeof(visited));
    cout << solve(0, 14, 14, 14, 14, 14) << endl;
    return 0;
}
