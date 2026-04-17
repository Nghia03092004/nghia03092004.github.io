#include <bits/stdc++.h>
using namespace std;

int main() {
    // Paper-strip game: count winning positions (G(n) != 0) for 1 <= n <= 10^6
    // Grundy values become periodic with period 34 starting at n=53

    const int LIMIT = 1000000;
    const int COMPUTE_UP_TO = 200;

    vector<int> G(COMPUTE_UP_TO + 1, 0);

    for (int n = 2; n <= COMPUTE_UP_TO; n++) {
        set<int> reachable;
        for (int i = 0; i <= n - 2; i++) {
            reachable.insert(G[i] ^ G[n - 2 - i]);
        }
        int mex = 0;
        while (reachable.count(mex)) mex++;
        G[n] = mex;
    }

    int period = 34;
    int offset = 53;

    // Verify periodicity
    for (int i = offset; i + period <= COMPUTE_UP_TO; i++) {
        assert(G[i] == G[i + period]);
    }

    // Count winning positions (G(n) != 0) for n = 1 to offset-1
    long long count = 0;
    for (int i = 1; i < offset && i <= LIMIT; i++) {
        if (G[i] != 0) count++;
    }

    if (LIMIT >= offset) {
        // Wins per period
        int wins_per_period = 0;
        for (int i = offset; i < offset + period; i++) {
            if (G[i] != 0) wins_per_period++;
        }

        long long remaining = (long long)(LIMIT) - offset + 1;
        long long full_periods = remaining / period;
        long long leftover = remaining % period;

        count += full_periods * wins_per_period;

        for (int i = 0; i < leftover; i++) {
            if (G[offset + i] != 0) count++;
        }
    }

    cout << count << endl;
    return 0;
}
