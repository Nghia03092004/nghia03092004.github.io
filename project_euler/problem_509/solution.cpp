#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 500;

    // Sieve proper divisors
    vector<vector<int>> pdivs(N + 1);
    for (int d = 1; d <= N; d++) {
        for (int m = 2 * d; m <= N; m += d) {
            pdivs[m].push_back(d);
        }
    }

    // Compute Grundy values
    vector<int> g(N + 1, 0);
    for (int n = 2; n <= N; n++) {
        set<int> reachable;
        for (int d : pdivs[n]) {
            reachable.insert(g[n - d]);
        }
        int mex = 0;
        while (reachable.count(mex)) mex++;
        g[n] = mex;
    }

    // Count winning positions
    int winning = 0;
    for (int n = 1; n <= N; n++) {
        if (g[n] > 0) winning++;
    }

    // Output
    cout << "First 30 positions:" << endl;
    for (int n = 1; n <= 30; n++) {
        cout << "  n=" << n << ": g=" << g[n]
             << " [" << (g[n] > 0 ? "W" : "L") << "]" << endl;
    }

    cout << "\nWinning positions for n=1.." << N << ": " << winning << endl;

    // List losing positions
    cout << "\nLosing positions: ";
    for (int n = 1; n <= N; n++) {
        if (g[n] == 0) cout << n << " ";
    }
    cout << endl;

    return 0;
}
