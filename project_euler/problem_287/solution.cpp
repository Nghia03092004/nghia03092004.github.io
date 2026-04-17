#include <bits/stdc++.h>
using namespace std;

const int N = 24;
const long long C = 1LL << (N - 1);  // center = 2^23
const long long R2 = 1LL << (2 * (N - 1));  // radius^2 = 2^46

bool inside(long long x, long long y) {
    long long dx = x - C;
    long long dy = y - C;
    return dx * dx + dy * dy <= R2;
}

long long solve(long long x0, long long y0, long long s) {
    if (s == 1) {
        return 2;  // single pixel: either 10 or 11
    }

    // Check all four corners of the pixel region [x0, x0+s-1] x [y0, y0+s-1]
    long long x1 = x0 + s - 1;
    long long y1 = y0 + s - 1;

    bool c00 = inside(x0, y0);
    bool c10 = inside(x1, y0);
    bool c01 = inside(x0, y1);
    bool c11 = inside(x1, y1);

    if (c00 && c10 && c01 && c11) {
        // All corners inside => all black (since disk is convex, all interior pixels are inside too)
        return 2;
    }

    // Check if entirely outside: find closest point in the square to center
    long long cx = max(x0, min(C, x1));
    long long cy = max(y0, min(C, y1));
    if (!inside(cx, cy)) {
        // Closest point is outside the disk => all white
        return 2;
    }

    // Mixed: subdivide
    long long h = s / 2;
    return 1 + solve(x0, y0, h) + solve(x0 + h, y0, h) +
               solve(x0, y0 + h, h) + solve(x0 + h, y0 + h, h);
}

int main() {
    long long S = 1LL << N;
    cout << solve(0, 0, S) << endl;
    return 0;
}
