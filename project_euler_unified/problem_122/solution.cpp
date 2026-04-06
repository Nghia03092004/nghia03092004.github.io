#include <bits/stdc++.h>
using namespace std;

int target;
vector<int> chain;

bool dfs(int depth, int maxDepth) {
    int cur = chain.back();
    if (cur == target) return true;
    if (depth == maxDepth) return false;
    if ((long long)cur << (maxDepth - depth) < target) return false;

    for (int i = depth; i >= 0; i--) {
        int nxt = cur + chain[i];
        if (nxt <= cur || nxt > target) continue;
        chain.push_back(nxt);
        if (dfs(depth + 1, maxDepth)) return true;
        chain.pop_back();
    }
    return false;
}

int shortestChain(int n) {
    if (n == 1) return 0;
    chain.clear();
    chain.push_back(1);

    int lb = 0;
    while ((1 << lb) < n) lb++;

    for (int maxDepth = lb; ; maxDepth++)
        if (dfs(0, maxDepth)) return maxDepth;
}

int main() {
    int ans = 0;
    for (int n = 1; n <= 200; n++) {
        target = n;
        ans += shortestChain(n);
    }
    cout << ans << endl;
    return 0;
}
