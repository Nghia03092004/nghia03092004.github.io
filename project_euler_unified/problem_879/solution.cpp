#include <bits/stdc++.h>
using namespace std;

// Problem 879: Touch-screen Password
// Count valid passwords on a 4x4 grid

typedef long long ll;

const int N = 4;
const int TOTAL = N * N;  // 16 spots

// Coordinates
int row[16], col[16];

// intermediate[i][j] = bitmask of spots that lie strictly between spot i and spot j
int intermediate[16][16];

void precompute() {
    for (int i = 0; i < TOTAL; i++) {
        row[i] = i / N;
        col[i] = i % N;
    }

    memset(intermediate, 0, sizeof(intermediate));
    for (int i = 0; i < TOTAL; i++) {
        for (int j = 0; j < TOTAL; j++) {
            if (i == j) continue;
            int dr = row[j] - row[i];
            int dc = col[j] - col[i];
            int g = __gcd(abs(dr), abs(dc));
            if (g <= 1) continue;
            int sr = dr / g, sc = dc / g;
            for (int k = 1; k < g; k++) {
                int r = row[i] + k * sr;
                int c = col[i] + k * sc;
                int idx = r * N + c;
                intermediate[i][j] |= (1 << idx);
            }
        }
    }
}

ll total_count = 0;

// DFS: current position, visited bitmask, path length
void dfs(int pos, int visited, int len) {
    if (len >= 2) {
        total_count++;
    }

    for (int next = 0; next < TOTAL; next++) {
        if (visited & (1 << next)) continue;
        if (pos == next) continue;

        int inter = intermediate[pos][next];
        // All intermediate spots must already be visited
        int unvisited_inter = inter & (~visited);

        if (unvisited_inter != 0) {
            // There are unvisited intermediate spots - they get auto-included
            // But this means the actual move goes through them first
            // The rule says: intermediate spots ARE included in the sequence
            // So we can't jump over unvisited spots - they become part of the path
            // Actually, re-reading the problem: the intermediate spot is included
            // in the password. So going from pos to next, if there's an unvisited
            // intermediate spot, it gets added to the path first.
            // This means we need to handle the full chain.

            // For simplicity with the "auto-include" rule:
            // If we try to go from pos to next, any unvisited intermediate
            // spots are automatically visited in order. But the problem says
            // the user traces a straight line - intermediate spots are added.
            // So we should only allow direct moves where either:
            // (a) there are no intermediate spots, or
            // (b) all intermediate spots are already visited
            // Because if an unvisited intermediate spot exists, the finger
            // would stop there first (making it a shorter segment).

            continue;  // Can't skip over unvisited intermediate spots
        }

        int new_visited = visited | (1 << next);
        dfs(next, new_visited, len + 1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    precompute();

    total_count = 0;
    for (int start = 0; start < TOTAL; start++) {
        dfs(start, 1 << start, 1);
    }

    cout << total_count << endl;

    return 0;
}
