#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 557: Cutting Triangles
 *
 * Find all quadruples (a,b,c,d) with a,b,c,d >= 1, b <= c, ad = bc,
 * and a+b+c+d <= N. S(N) = sum of (a+b+c+d) for all such quadruples.
 *
 * Key constraint: ad = bc => d = bc/a, so a | bc.
 *
 * We iterate over a and b, and for each, find valid c values.
 * From ad = bc and d = bc/a, we need:
 *   - a | bc
 *   - c >= b (since b <= c)
 *   - a + b + c + bc/a <= N
 *
 * Let g = gcd(a, b), a = g*a', b = g*b' with gcd(a',b')=1.
 * Then d = bc/a = b'c/a'. Since gcd(a',b')=1, we need a' | c.
 * So c = a' * k for some integer k >= 1.
 * Also b <= c => g*b' <= g*a'*k => b' <= a'*k => k >= ceil(b'/a').
 * But since b' >= 1 and a' >= 1, k >= 1 always works if a' >= b'.
 *
 * d = b' * a' * k / a' = b' * k
 *
 * Wait, let me redo: a = g*a', b = g*b', gcd(a',b')=1.
 * d = bc/a = g*b' * c / (g*a') = b'*c/a'.
 * So a' | c. Let c = a' * k.
 * Then d = b' * k.
 * Constraint b <= c: g*b' <= a'*k => k >= g*b'/a' (but k integer)
 * Actually c is just a'*k, not g*a'*k. Let me be more careful.
 *
 * a, b, c, d are the actual areas.
 * ad = bc. Let g = gcd(a,b). a = g*a', b = g*b', gcd(a',b')=1.
 * d = bc/a = g*b'*c/(g*a') = b'*c/a'.
 * Need a'|c. Let c = a'*t for positive integer t.
 * Then d = b'*t.
 * Constraint b <= c: b <= a'*t => g*b' <= a'*t => t >= g*b'/a'.
 * Since t is integer: t >= ceil(g*b'/a').
 * Sum constraint: a + b + c + d <= N
 *   g*a' + g*b' + a'*t + b'*t <= N
 *   g*(a'+b') + t*(a'+b') <= N
 *   (a'+b')*(g+t) <= N
 *   t <= N/(a'+b') - g
 *
 * So we iterate over g, a', b' with gcd(a',b')=1, a' >= 1, b' >= 1.
 * For each, t ranges from max(1, ceil(g*b'/a')) to floor(N/(a'+b') - g).
 * Total area T = (a'+b')*(g+t).
 * S(N) = sum of T over all valid (g, a', b', t).
 */

int main() {
    long long N = 10000;
    long long S = 0;

    // Iterate over a' and b' with gcd(a',b') = 1
    for (long long apb = 2; apb <= N; apb++) { // a' + b'
        for (long long bp = 1; bp < apb; bp++) {
            long long ap = apb - bp;
            if (ap < 1) continue;
            if (__gcd(ap, bp) != 1) continue;

            // For each g >= 1:
            // t_min = max(1, ceil(g*bp/ap))
            // t_max = floor(N/apb - g)
            // Need t_max >= t_min, i.e., N/apb - g >= t_min
            // g + t <= N/apb => g <= N/apb - 1

            long long max_g = N / apb - 1;
            for (long long g = 1; g <= max_g; g++) {
                long long t_min = max(1LL, (g * bp + ap - 1) / ap);
                long long t_max = N / apb - g;
                if (t_max < t_min) continue;

                // Sum T = apb * (g + t) for t from t_min to t_max
                // = apb * sum_{t=t_min}^{t_max} (g + t)
                // = apb * ((t_max - t_min + 1) * g + sum_{t=t_min}^{t_max} t)
                // = apb * ((t_max - t_min + 1) * g + (t_min + t_max) * (t_max - t_min + 1) / 2)
                long long cnt = t_max - t_min + 1;
                long long sum_t = (t_min + t_max) * cnt / 2;
                S += apb * (cnt * g + sum_t);
            }
        }
    }

    cout << S << endl;
    return 0;
}
