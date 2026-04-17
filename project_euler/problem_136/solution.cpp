#include <bits/stdc++.h>
using namespace std;

int main() {
    // n = y*(4d - y) where d in (y/4, y)
    // Let u = 4d - y, then n = y*u, u in (0, 3y), u+y = 0 mod 4
    // Count n < N with exactly one (y,u) factorization satisfying constraints.

    const int N = 50000000;
    vector<int> cnt(N, 0);

    for (int y = 1; y < N; y++) {
        // u must satisfy: u > 0, u < 3y, y*u < N, (u+y) % 4 == 0
        // u starts at first positive value with (u+y) % 4 == 0
        int u_start = (4 - (y % 4)) % 4;
        if (u_start == 0) u_start = 4;
        // Actually u > 0 and (u+y) % 4 == 0
        // So u % 4 == (-y) % 4 == (4 - y%4) % 4
        int rem = (4 - y % 4) % 4;
        u_start = rem;
        if (u_start == 0) u_start = 4;

        long long u_max = min((long long)3 * y - 1, (long long)(N - 1) / y);

        for (long long u = u_start; u <= u_max; u += 4) {
            cnt[y * u]++;
        }
    }

    int answer = 0;
    for (int i = 1; i < N; i++) {
        if (cnt[i] == 1) answer++;
    }

    cout << answer << endl;
    return 0;
}
