#include <bits/stdc++.h>
using namespace std;

int main() {
    const int N = 120000;
    vector<int> rad(N, 1);

    // Sieve for radicals
    for (int p = 2; p < N; p++) {
        if (rad[p] == 1) { // p is prime
            for (int m = p; m < N; m += p) {
                rad[m] *= p;
            }
        }
    }

    // Sort indices 1..N-1 by radical (skip 0)
    vector<int> sorted_by_rad;
    sorted_by_rad.reserve(N - 1);
    for (int i = 1; i < N; i++) sorted_by_rad.push_back(i);
    sort(sorted_by_rad.begin(), sorted_by_rad.end(),
         [&](int x, int y) { return rad[x] < rad[y]; });

    long long total = 0;

    for (int c = 3; c < N; c++) {
        long long threshold = (long long)c / rad[c];
        if (threshold <= 1) continue;

        for (int i = 0; i < (int)sorted_by_rad.size(); i++) {
            int a = sorted_by_rad[i];
            if (rad[a] >= threshold) break;
            if (a >= c) continue;
            int b = c - a;
            if (a >= b) continue;
            if ((long long)rad[a] * rad[b] >= threshold) continue;
            if (__gcd(rad[a], rad[b]) != 1) continue;
            total += c;
        }
    }

    cout << total << endl;
    return 0;
}
