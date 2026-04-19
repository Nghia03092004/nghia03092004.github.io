// IOI 1994 - The Clock
// BFS over all 4^9 states to find shortest move sequence
// Each of 9 moves rotates a subset of clocks 90 degrees clockwise
// Time: O(4^9 * 9), Space: O(4^9)
#include <bits/stdc++.h>
using namespace std;

// Clocks affected by each move (0-indexed clocks)
const int moves[9][5] = {
    {0, 1, 3, 4, -1},   // Move 1: clocks 1,2,4,5
    {0, 1, 2, -1, -1},  // Move 2: clocks 1,2,3
    {1, 2, 4, 5, -1},   // Move 3: clocks 2,3,5,6
    {0, 3, 6, -1, -1},  // Move 4: clocks 1,4,7
    {1, 3, 4, 5, 7},    // Move 5: clocks 2,4,5,6,8
    {2, 5, 8, -1, -1},  // Move 6: clocks 3,6,9
    {3, 4, 6, 7, -1},   // Move 7: clocks 4,5,7,8
    {6, 7, 8, -1, -1},  // Move 8: clocks 7,8,9
    {4, 5, 7, 8, -1}    // Move 9: clocks 5,6,8,9
};

int encode(int c[]) {
    int s = 0;
    for (int i = 0; i < 9; i++)
        s = s * 4 + c[i];
    return s;
}

void decode(int s, int c[]) {
    for (int i = 8; i >= 0; i--) {
        c[i] = s % 4;
        s /= 4;
    }
}

void applyMove(int c[], int m) {
    for (int j = 0; j < 5 && moves[m][j] != -1; j++)
        c[moves[m][j]] = (c[moves[m][j]] + 1) % 4;
}

int main() {
    int clk[9];
    for (int i = 0; i < 9; i++) {
        int x;
        scanf("%d", &x);
        // Input: 12, 3, 6, or 9 (hours)
        // Convert to 0,1,2,3: 12->0, 3->1, 6->2, 9->3
        clk[i] = x % 12 / 3;
    }

    int start = encode(clk);
    int target = 0; // all clocks at 12

    if (start == target) {
        printf("\n");
        return 0;
    }

    // BFS
    unordered_map<int, int> dist;
    unordered_map<int, pair<int, int>> parent; // state -> (prev_state, move)
    queue<int> q;
    q.push(start);
    dist[start] = 0;

    while (!q.empty()) {
        int u = q.front(); q.pop();

        for (int m = 0; m < 9; m++) {
            int tmp[9];
            decode(u, tmp);
            applyMove(tmp, m);
            int v = encode(tmp);
            if (dist.find(v) == dist.end()) {
                dist[v] = dist[u] + 1;
                parent[v] = {u, m};
                if (v == target) {
                    // Reconstruct path
                    vector<int> path;
                    int cur = target;
                    while (cur != start) {
                        path.push_back(parent[cur].second + 1);
                        cur = parent[cur].first;
                    }
                    reverse(path.begin(), path.end());
                    for (int i = 0; i < (int)path.size(); i++) {
                        if (i) printf(" ");
                        printf("%d", path[i]);
                    }
                    printf("\n");
                    return 0;
                }
                q.push(v);
            }
        }
    }

    return 0;
}
