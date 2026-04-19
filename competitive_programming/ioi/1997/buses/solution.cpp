// IOI 1997 - The Buses
// Backtracking: find minimum number of bus routes explaining all arrivals
// Each route: start time, interval, count >= 2
// Time: exponential with pruning, Space: O(n)
#include <bits/stdc++.h>
using namespace std;

int n;
int cnt[60]; // count of each arrival time available
int bestRoutes;

struct Route {
    int start, interval, count;
};
vector<Route> bestSolution, currentSolution;

// Check if a route (start, interval, count) is valid with current counts
bool canUse(int s, int d, int c) {
    for (int i = 0; i < c; i++) {
        int t = s + i * d;
        if (t > 59 || cnt[t] <= 0) return false;
    }
    return true;
}

void useRoute(int s, int d, int c, int delta) {
    for (int i = 0; i < c; i++)
        cnt[s + i * d] += delta;
}

int remaining() {
    int r = 0;
    for (int i = 0; i < 60; i++) r += cnt[i];
    return r;
}

void solve() {
    if (remaining() == 0) {
        if ((int)currentSolution.size() < bestRoutes) {
            bestRoutes = (int)currentSolution.size();
            bestSolution = currentSolution;
        }
        return;
    }

    if ((int)currentSolution.size() + 1 >= bestRoutes) return;

    // Find first unassigned time
    int first = -1;
    for (int i = 0; i < 60; i++)
        if (cnt[i] > 0) { first = i; break; }

    // Try all routes starting at 'first'
    for (int d = 1; d <= 59; d++) {
        // Find max count for this interval
        int maxc = 0;
        for (int t = first; t <= 59; t += d) {
            if (cnt[t] > 0) maxc++;
            else break;
        }
        if (maxc < 2) continue;

        // Try from largest count downward (greedy: cover more with one route)
        for (int c = maxc; c >= 2; c--) {
            if (canUse(first, d, c)) {
                useRoute(first, d, c, -1);
                currentSolution.push_back({first, d, c});
                solve();
                currentSolution.pop_back();
                useRoute(first, d, c, +1);
            }
        }
    }
}

int main() {
    scanf("%d", &n);
    memset(cnt, 0, sizeof(cnt));
    for (int i = 0; i < n; i++) {
        int t;
        scanf("%d", &t);
        cnt[t]++;
    }

    bestRoutes = n; // worst case
    solve();

    printf("%d\n", bestRoutes);
    for (auto& r : bestSolution)
        printf("%d %d %d\n", r.start, r.interval, r.count);

    return 0;
}
