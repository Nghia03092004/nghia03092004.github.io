/*
 * Project Euler Problem 252: Convex Holes
 *
 * Find the largest area convex hole among 500 LCG-generated points.
 * A convex hole is a convex polygon whose vertices are from the point set
 * and whose interior contains no other points.
 *
 * Algorithm: For each anchor p0, sort remaining points by polar angle,
 * then DP on (last two vertices) tracking the first vertex for closure.
 * Empty-triangle tests use precomputed bitsets (64-bit words).
 *
 * Answer: 104924.0
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef pair<ll, ll> pll;

ll cross(pll O, pll A, pll B) {
    return (A.first - O.first) * (B.second - O.second)
         - (A.second - O.second) * (B.first - O.first);
}

int main() {
    // Generate points
    vector<pll> pts;
    ll s = 290797;
    vector<ll> t;
    for (int i = 0; i < 1000; i++) {
        s = (s * s) % 50515093LL;
        t.push_back(s % 2000 - 1000);
    }
    for (int k = 0; k < 500; k++)
        pts.push_back({t[2 * k], t[2 * k + 1]});

    int n = pts.size();
    const int WORDS = (n + 63) / 64;

    // Precompute bitsets: B[i][j] = points strictly left of directed edge i->j
    vector<vector<vector<uint64_t>>> B(n, vector<vector<uint64_t>>(n, vector<uint64_t>(WORDS, 0)));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            for (int p = 0; p < n; p++) {
                if (p == i || p == j) continue;
                if (cross(pts[i], pts[j], pts[p]) > 0)
                    B[i][j][p / 64] |= (1ULL << (p % 64));
            }
        }

    auto triangle_empty = [&](int a, int b, int c) -> bool {
        for (int w = 0; w < WORDS; w++)
            if (B[a][b][w] & B[b][c][w] & B[c][a][w]) return false;
        return true;
    };

    ll best = 0;

    for (int p0 = 0; p0 < n; p0++) {
        vector<int> idx;
        for (int i = 0; i < n; i++)
            if (i != p0) idx.push_back(i);

        sort(idx.begin(), idx.end(), [&](int a, int b) {
            ll ax = pts[a].first - pts[p0].first, ay = pts[a].second - pts[p0].second;
            ll bx = pts[b].first - pts[p0].first, by = pts[b].second - pts[p0].second;
            int qa = (ay > 0 || (ay == 0 && ax > 0)) ? 0 : 1;
            int qb = (by > 0 || (by == 0 && bx > 0)) ? 0 : 1;
            if (qa != qb) return qa < qb;
            ll cr = ax * by - ay * bx;
            if (cr != 0) return cr > 0;
            return ax * ax + ay * ay < bx * bx + by * by;
        });

        int m = idx.size();
        vector<int> first_vtx(m * m, -1);
        auto FV = [&](int j, int k) -> int& { return first_vtx[j * m + k]; };
        vector<ll> dp(m * m, -1);
        auto DP = [&](int j, int k) -> ll& { return dp[j * m + k]; };

        // Base triangles
        for (int j = 0; j < m; j++)
            for (int k2 = j + 1; k2 < m; k2++) {
                ll cr = cross(pts[p0], pts[idx[j]], pts[idx[k2]]);
                if (cr <= 0) continue;
                if (!triangle_empty(p0, idx[j], idx[k2])) continue;
                DP(j, k2) = cr;
                FV(j, k2) = j;
                best = max(best, cr);
            }

        // Extend
        for (int k2 = 0; k2 < m; k2++)
            for (int j = 0; j < k2; j++) {
                if (DP(j, k2) < 0) continue;
                int f = FV(j, k2);
                for (int l = k2 + 1; l < m; l++) {
                    ll cr1 = cross(pts[idx[j]], pts[idx[k2]], pts[idx[l]]);
                    if (cr1 <= 0) continue;
                    ll cr2 = cross(pts[p0], pts[idx[k2]], pts[idx[l]]);
                    if (cr2 <= 0) continue;
                    if (!triangle_empty(p0, idx[k2], idx[l])) continue;
                    if (cross(pts[p0], pts[idx[f]], pts[idx[l]]) <= 0) continue;

                    ll new_area = DP(j, k2) + cr2;
                    if (new_area > DP(k2, l)) {
                        DP(k2, l) = new_area;
                        FV(k2, l) = f;
                    }
                    if (cross(pts[idx[k2]], pts[idx[l]], pts[p0]) > 0 &&
                        cross(pts[idx[l]], pts[p0], pts[idx[f]]) > 0)
                        best = max(best, new_area);
                }
            }
    }

    printf("%.1f\n", best / 2.0);
    return 0;
}
