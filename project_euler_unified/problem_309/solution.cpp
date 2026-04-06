#include <bits/stdc++.h>
using namespace std;

int main() {
    // Integer Ladders
    // Two ladders of lengths x, y lean against opposite walls of alley width w.
    // x^2 = w^2 + p^2, y^2 = w^2 + q^2. Crossing height h = pq/(p+q) integer.
    // Count (x,y,h) with 0 < x < y < 1000000, all integers.
    //
    // Strategy: for each w, find all Pythagorean triples (w, p, hyp) with hyp < LIMIT.
    // For each pair of such triples sharing the same w, check if h is integer.

    const long long LIMIT = 1000000;

    // For each w, store pairs (p, hyp) where w^2 + p^2 = hyp^2 and hyp < LIMIT.
    // Generate from primitive Pythagorean triples.
    // Primitive: (m^2-n^2, 2mn, m^2+n^2), m>n>0, gcd(m,n)=1, m-n odd.
    // w can be either leg.

    // We need hyp < LIMIT, so m^2+n^2 < LIMIT for primitive triples,
    // and d*(m^2+n^2) < LIMIT for multiples.

    // Group by w: for each w, list of (p, hyp) pairs.
    // Use a map since w can be up to ~LIMIT.
    unordered_map<int, vector<pair<int,int>>> by_w;

    for (long long m = 2; m * m < LIMIT; m++) {
        for (long long n = 1; n < m; n++) {
            if ((m - n) % 2 == 0) continue;
            if (__gcd(m, n) != 1LL) continue;

            long long w1 = m * m - n * n;
            long long p1 = 2 * m * n;
            long long hyp = m * m + n * n;

            // Multiples: d * (w1, p1, hyp)
            for (long long d = 1; d * hyp < LIMIT; d++) {
                long long w = d * w1, p = d * p1, h = d * hyp;
                by_w[(int)w].push_back({(int)p, (int)h});
            }
            // Also: d * (p1, w1, hyp) - swap legs
            for (long long d = 1; d * hyp < LIMIT; d++) {
                long long w = d * p1, p = d * w1, h = d * hyp;
                by_w[(int)w].push_back({(int)p, (int)h});
            }
        }
    }

    long long answer = 0;

    for (auto& [w, legs] : by_w) {
        int sz = legs.size();
        if (sz < 2) continue;

        // For each pair of triples sharing this w:
        // (p1, hyp1) and (p2, hyp2) where hyp1 < hyp2
        // Check: h = p1*p2/(p1+p2) is integer
        // And hyp1 < hyp2 < LIMIT (already ensured by generation)
        // Count as triplet (hyp1, hyp2, h)

        // Need to avoid double-counting: (x,y) = (hyp1, hyp2) with x < y.
        // A pair of triples (p1,hyp1) and (p2,hyp2) with hyp1 < hyp2 gives one triplet.
        // Same (p1,hyp1) and (p2,hyp2) gives same (x,y) regardless of w.
        // But different w values give different configurations.
        // Actually: the problem counts (x, y, h) triplets. Same x,y might give different h
        // with different w. So each (w, pair) is distinct if h differs.
        // Actually: for given x and y, w is uniquely determined (the crossing geometry
        // determines w from x, y, h). Wait no: for given (x, y), there might be multiple
        // valid w values. But each (x, y, h) triplet is unique because h determines w.
        // h = pq/(p+q). p = sqrt(x^2-w^2), q = sqrt(y^2-w^2). Different w -> different h.
        // So each (w, pair) gives a unique (x, y, h).

        for (int i = 0; i < sz; i++) {
            for (int j = i + 1; j < sz; j++) {
                long long p1 = legs[i].first, hyp1 = legs[i].second;
                long long p2 = legs[j].first, hyp2 = legs[j].second;
                // Ensure x < y (smaller hypotenuse first)
                // hyp1 and hyp2 might be in any order
                if (hyp1 == hyp2) continue; // x < y strictly

                long long g = __gcd(p1, p2);
                long long alpha = p1 / g;
                long long beta = p2 / g;
                // h = g * alpha * beta / (alpha + beta). Integer iff (alpha+beta) | g.
                if (g % (alpha + beta) == 0) {
                    answer++;
                }
            }
        }
    }

    cout << answer << endl;
    return 0;
}
