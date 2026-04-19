// IOI 2002 - Bus Terminals
// Partition N points into 2 groups, place a terminal for each group.
// Minimize the maximum Manhattan distance from any point to its terminal.
// Approach: Rotate coordinates (u=x+y, v=x-y) to convert Manhattan to L_inf.
// Binary search on answer R, check if points can be covered by two 2R-side squares.
// Split by u-sorted or v-sorted order and check prefix/suffix bounding boxes.
// Complexity: O(N log N + N log C) time, O(N) space.

#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    if (N == 0) {
        cout << 0 << "\n";
        return 0;
    }

    vector<long long> u(N), v(N);
    for (int i = 0; i < N; i++) {
        long long x, y;
        cin >> x >> y;
        u[i] = x + y;
        v[i] = x - y;
    }

    if (N == 1) {
        cout << 0 << "\n";
        return 0;
    }

    // Sort indices by u-coordinate
    vector<int> idx(N);
    iota(idx.begin(), idx.end(), 0);
    sort(idx.begin(), idx.end(), [&](int a, int b) { return u[a] < u[b]; });

    // Precompute suffix min/max of v (sorted by u)
    vector<long long> suffMinV(N + 1), suffMaxV(N + 1);
    suffMinV[N] = LLONG_MAX;
    suffMaxV[N] = LLONG_MIN;
    for (int i = N - 1; i >= 0; i--) {
        suffMinV[i] = min(suffMinV[i + 1], v[idx[i]]);
        suffMaxV[i] = max(suffMaxV[i + 1], v[idx[i]]);
    }

    // Check if points can be split by u-sorted order into two groups,
    // each fitting in a 2R x 2R square
    auto checkByU = [&](long long twoR) -> bool {
        long long prefMinV = LLONG_MAX, prefMaxV = LLONG_MIN;
        for (int i = 0; i < N; i++) {
            prefMinV = min(prefMinV, v[idx[i]]);
            prefMaxV = max(prefMaxV, v[idx[i]]);
            long long prefUR = u[idx[i]] - u[idx[0]];
            long long prefVR = prefMaxV - prefMinV;
            if (prefUR <= twoR && prefVR <= twoR) {
                if (i == N - 1) return true;
                long long sufUR = u[idx[N - 1]] - u[idx[i + 1]];
                long long sufVR = suffMaxV[i + 1] - suffMinV[i + 1];
                if (sufUR <= twoR && sufVR <= twoR) return true;
            }
        }
        return false;
    };

    // Sort indices by v-coordinate
    vector<int> idxV(N);
    iota(idxV.begin(), idxV.end(), 0);
    sort(idxV.begin(), idxV.end(), [&](int a, int b) { return v[a] < v[b]; });

    // Precompute suffix min/max of u (sorted by v)
    vector<long long> suffMinU(N + 1), suffMaxU(N + 1);
    suffMinU[N] = LLONG_MAX;
    suffMaxU[N] = LLONG_MIN;
    for (int i = N - 1; i >= 0; i--) {
        suffMinU[i] = min(suffMinU[i + 1], u[idxV[i]]);
        suffMaxU[i] = max(suffMaxU[i + 1], u[idxV[i]]);
    }

    // Check if points can be split by v-sorted order
    auto checkByV = [&](long long twoR) -> bool {
        long long prefMinU = LLONG_MAX, prefMaxU = LLONG_MIN;
        for (int i = 0; i < N; i++) {
            prefMinU = min(prefMinU, u[idxV[i]]);
            prefMaxU = max(prefMaxU, u[idxV[i]]);
            long long prefVR = v[idxV[i]] - v[idxV[0]];
            long long prefUR = prefMaxU - prefMinU;
            if (prefVR <= twoR && prefUR <= twoR) {
                if (i == N - 1) return true;
                long long sufVR = v[idxV[N - 1]] - v[idxV[i + 1]];
                long long sufUR = suffMaxU[i + 1] - suffMinU[i + 1];
                if (sufVR <= twoR && sufUR <= twoR) return true;
            }
        }
        return false;
    };

    auto feasible = [&](long long twoR) -> bool {
        return checkByU(twoR) || checkByV(twoR);
    };

    // Binary search on 2R (the side length of covering squares)
    long long lo = 0, hi = 4000000000LL;
    while (lo < hi) {
        long long mid = lo + (hi - lo) / 2;
        if (feasible(mid))
            hi = mid;
        else
            lo = mid + 1;
    }

    // lo is the minimum 2R in L_inf = minimum max Manhattan distance
    cout << lo << "\n";

    return 0;
}
