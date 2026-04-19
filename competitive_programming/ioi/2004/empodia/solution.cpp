// IOI 2004 - Empodia
// Find all maximal empodia in a permutation.
// An empodio [l,r] requires the set {a[l],...,a[r]} to be consecutive integers,
// with a[l] = min and a[r] = max. Maximal means not contained in another.
// O(N log N) using sparse table for range min/max queries.
#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];

    if (N <= 1) {
        cout << 0 << "\n";
        return 0;
    }

    // Key insight: empodio endpoints share same c[i] = a[i] - i value.
    vector<int> c(N);
    for (int i = 0; i < N; i++) c[i] = a[i] - i;

    // Group indices by c-value
    map<int, vector<int>> byC;
    for (int i = 0; i < N; i++) byC[c[i]].push_back(i);

    // Sparse table for range min and range max of a[]
    int LOG = 1;
    while ((1 << LOG) <= N) LOG++;
    vector<vector<int>> rmn(LOG, vector<int>(N));
    vector<vector<int>> rmx(LOG, vector<int>(N));
    for (int i = 0; i < N; i++) rmn[0][i] = rmx[0][i] = a[i];
    for (int k = 1; k < LOG; k++) {
        for (int i = 0; i + (1 << k) <= N; i++) {
            rmn[k][i] = min(rmn[k - 1][i], rmn[k - 1][i + (1 << (k - 1))]);
            rmx[k][i] = max(rmx[k - 1][i], rmx[k - 1][i + (1 << (k - 1))]);
        }
    }
    auto qmin = [&](int l, int r) {
        int k = __lg(r - l + 1);
        return min(rmn[k][l], rmn[k][r - (1 << k) + 1]);
    };
    auto qmax = [&](int l, int r) {
        int k = __lg(r - l + 1);
        return max(rmx[k][l], rmx[k][r - (1 << k) + 1]);
    };

    // Find all valid empodia: consecutive same-c pairs where a[l]=min, a[r]=max
    vector<pair<int, int>> candidates;
    for (auto& [cv, positions] : byC) {
        for (int idx = 0; idx + 1 < (int)positions.size(); idx++) {
            int l = positions[idx], r = positions[idx + 1];
            if (a[l] < a[r] && qmin(l, r) == a[l] && qmax(l, r) == a[r]) {
                candidates.push_back({l, r});
            }
        }
    }

    // Filter for maximal: sort by l asc, r desc; two-pass filter
    sort(candidates.begin(), candidates.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.first != b.first ? a.first < b.first : a.second > b.second;
    });
    candidates.erase(unique(candidates.begin(), candidates.end()), candidates.end());

    // Pass 1: keep only those with r > maxR so far (not nested from left)
    vector<pair<int, int>> pass1;
    int maxR = -1;
    for (auto& [l, r] : candidates) {
        if (r > maxR) {
            pass1.push_back({l, r});
            maxR = r;
        }
    }

    // Pass 2: sort by r desc, l asc; keep only those with l < minL so far
    sort(pass1.begin(), pass1.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        return a.second != b.second ? a.second > b.second : a.first < b.first;
    });
    vector<pair<int, int>> result;
    int minL = INT_MAX;
    for (auto& [l, r] : pass1) {
        if (l < minL) {
            result.push_back({l, r});
            minL = l;
        }
    }

    sort(result.begin(), result.end());

    cout << result.size() << "\n";
    for (auto& [l, r] : result) {
        cout << l << " " << r << "\n";
    }

    return 0;
}
