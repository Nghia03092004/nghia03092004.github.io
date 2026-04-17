#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 729: Range of Periodic Sequence
 *
 * a_{n+1} = a_n - 1/a_n. Substitution a = cot(theta) gives theta -> 2*theta (mod pi).
 * Period-p orbits: theta_0 = m*pi/(2^p - 1), orbit under doubling map mod (2^p-1).
 * S(P) = sum of ranges of all exact-period-p orbits for p = 1..P.
 */

const double PI = acos(-1.0);

int main() {
    int P = 25;
    double total = 0.0;

    for (int p = 1; p <= P; p++) {
        long long N = (1LL << p) - 1;
        vector<bool> visited(N, false);

        for (long long m = 1; m < N; m++) {
            if (visited[m]) continue;

            vector<long long> orbit;
            long long x = m;
            while (!visited[x]) {
                visited[x] = true;
                orbit.push_back(x);
                x = (2 * x) % N;
            }

            if ((int)orbit.size() != p) continue;

            double mn = 1e18, mx = -1e18;
            for (long long idx : orbit) {
                double val = 1.0 / tan(idx * PI / N);
                mn = min(mn, val);
                mx = max(mx, val);
            }
            total += mx - mn;
        }
    }

    printf("S(%d) = %.4f\n", P, total);
    return 0;
}
