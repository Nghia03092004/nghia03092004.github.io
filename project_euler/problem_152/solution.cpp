#include <bits/stdc++.h>
using namespace std;
typedef long double ld;

int main() {
    set<int> excluded = {
        79,73,71,67,61,59,53,47,43,41,37,74,31,62,29,58,23,46,69,
        19,38,57,76,34,51,26,65,78,11,22,33,44,55,66,77,49,25,50,
        75,27,54,17,68
    };

    vector<int> candidates;
    for (int k = 2; k <= 80; k++)
        if (excluded.find(k) == excluded.end())
            candidates.push_back(k);

    int n = candidates.size();
    ld target = 0.5L;

    vector<ld> vals(n);
    for (int i = 0; i < n; i++)
        vals[i] = 1.0L / ((ld)candidates[i] * candidates[i]);

    vector<ld> suffix(n + 1, 0);
    for (int i = n - 1; i >= 0; i--)
        suffix[i] = suffix[i + 1] + vals[i];

    int count = 0;
    ld eps = 1e-18L;

    function<void(int, ld)> dfs = [&](int idx, ld current) {
        if (fabsl(current - target) < eps) { count++; return; }
        if (idx >= n) return;
        ld need = target - current;
        if (need < -eps) return;
        if (suffix[idx] < need - eps) return;
        for (int i = idx; i < n; i++) {
            if (vals[i] > need + eps) continue;
            if (suffix[i] < need - eps) return;
            dfs(i + 1, current + vals[i]);
        }
    };

    dfs(0, 0.0L);
    printf("%d\n", count);
    return 0;
}
