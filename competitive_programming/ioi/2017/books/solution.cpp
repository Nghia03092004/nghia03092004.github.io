// IOI 2017 - Books
// Cycle decomposition + merge spans + starting position offset
// Time: O(n log n), Space: O(n)
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

long long minimum_walk(vector<int> p, int s) {
    int n = p.size();
    if (n <= 1) return 0;

    // Find non-trivial cycles and compute their spans
    vector<bool> visited(n, false);
    vector<pair<int,int>> cycleSpans;
    int globalL = n, globalR = -1;

    for (int i = 0; i < n; i++) {
        if (visited[i] || p[i] == i) {
            visited[i] = true;
            continue;
        }
        int lo = i, hi = i, j = i;
        while (!visited[j]) {
            visited[j] = true;
            lo = min(lo, j);
            hi = max(hi, j);
            j = p[j];
        }
        cycleSpans.push_back({lo, hi});
        globalL = min(globalL, lo);
        globalR = max(globalR, hi);
    }

    if (cycleSpans.empty()) return 0; // already sorted

    // Merge overlapping cycle spans
    sort(cycleSpans.begin(), cycleSpans.end());
    vector<pair<int,int>> merged;
    for (auto& [lo, hi] : cycleSpans) {
        if (!merged.empty() && lo <= merged.back().second) {
            merged.back().second = max(merged.back().second, hi);
        } else {
            merged.push_back({lo, hi});
        }
    }

    // Mandatory cost: 2 * (total length of merged ranges)
    ll mandatory = 0;
    for (auto& [lo, hi] : merged)
        mandatory += 2LL * (hi - lo);

    // Gap cost: gaps between merged ranges must be traversed twice
    ll gaps = 0;
    for (int i = 0; i + 1 < (int)merged.size(); i++)
        gaps += merged[i + 1].first - merged[i].second;

    // Starting position extra cost
    ll extra;
    if (s >= globalL && s <= globalR)
        extra = min((ll)(s - globalL), (ll)(globalR - s));
    else if (s < globalL)
        extra = globalL - s;
    else
        extra = s - globalR;

    return mandatory + 2 * gaps + extra;
}

int main() {
    int n, s;
    scanf("%d %d", &n, &s);
    vector<int> p(n);
    for (int i = 0; i < n; i++) scanf("%d", &p[i]);
    printf("%lld\n", minimum_walk(p, s));
    return 0;
}
