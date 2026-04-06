#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 2000;
    const int SZ = N * N;

    // Generate the lagged Fibonacci sequence
    vector<long long> s(SZ + 1);
    for (int k = 1; k <= 55; k++) {
        long long k2 = (long long)k;
        s[k] = ((100003LL - 200003LL * k2 + 300007LL * k2 % 1000000 * k2 % 1000000 * k2) % 1000000 + 1000000) % 1000000 - 500000;
    }
    // Recompute first 55 more carefully to avoid overflow
    for (int k = 1; k <= 55; k++) {
        long long k2 = (long long)k;
        long long val = 100003LL;
        val = (val - 200003LL * k2 % 1000000 + 1000000) % 1000000;
        // 300007 * k^3 mod 10^6
        long long kk = k2 % 1000000;
        long long k3 = kk * kk % 1000000 * kk % 1000000;
        long long term3 = 300007LL * k3 % 1000000;
        val = (val + term3) % 1000000;
        s[k] = val - 500000;
    }

    for (int k = 56; k <= SZ; k++) {
        s[k] = ((s[k - 24] + s[k - 55]) % 1000000 + 1000000) % 1000000 - 500000;
    }

    // Build the 2000x2000 table
    vector<vector<long long>> table(N, vector<long long>(N));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            table[i][j] = s[N * i + j + 1];

    // Kadane's algorithm
    auto kadane = [](const vector<long long>& a) -> long long {
        long long maxHere = 0, maxSoFar = LLONG_MIN;
        for (long long x : a) {
            maxHere = max(x, maxHere + x);
            maxSoFar = max(maxSoFar, maxHere);
        }
        return maxSoFar;
    };

    long long ans = LLONG_MIN;

    // Horizontal
    for (int i = 0; i < N; i++) {
        ans = max(ans, kadane(table[i]));
    }

    // Vertical
    for (int j = 0; j < N; j++) {
        vector<long long> col(N);
        for (int i = 0; i < N; i++) col[i] = table[i][j];
        ans = max(ans, kadane(col));
    }

    // Diagonal (top-left to bottom-right)
    for (int d = -(N - 1); d < N; d++) {
        vector<long long> diag;
        for (int i = max(0, -d); i < N && i + d < N; i++) {
            diag.push_back(table[i][i + d]);
        }
        if (!diag.empty()) ans = max(ans, kadane(diag));
    }

    // Anti-diagonal (top-right to bottom-left)
    for (int d = 0; d < 2 * N - 1; d++) {
        vector<long long> adiag;
        for (int i = max(0, d - N + 1); i < N && d - i >= 0 && d - i < N; i++) {
            adiag.push_back(table[i][d - i]);
        }
        if (!adiag.empty()) ans = max(ans, kadane(adiag));
    }

    cout << ans << endl;
    return 0;
}
