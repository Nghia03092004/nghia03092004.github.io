#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C;
    cin >> R >> C;

    vector<string> grid(R);
    int sr, sc, tr, tc;

    for(int i = 0; i < R; i++){
        cin >> grid[i];
        for(int j = 0; j < C; j++){
            if(grid[i][j] == 'S'){ sr = i; sc = j; grid[i][j] = '.'; }
            if(grid[i][j] == 'T'){ tr = i; tc = j; grid[i][j] = '.'; }
        }
    }

    // State: (row, col, parity of (horizontal_moves - vertical_moves))
    // horizontal move: diff += 1 (or -1, same absolute)
    // vertical move: diff -= 1 (or +1)
    // Actually, let's track diff = h_moves - v_moves.
    // Move right/left: diff changes by +1
    // Move up/down: diff changes by -1
    // We need diff = 0 at target, or more precisely |diff| mod 2 = 0
    // and we can adjust by 2 using detours.

    // BFS with state (r, c, parity) where parity = diff mod 2
    // parity 0: horizontal, parity 1: vertical
    // Move horizontally: parity flips (diff +/- 1)
    // Move vertically: parity flips (diff -/+ 1)
    // So ANY move flips the parity.
    // Starting parity = 0 (diff = 0).
    // We need parity = 0 at target.
    // This means we need an even number of total moves.
    // Manhattan distance = |sr-tr| + |sc-tc|. Any path has length >= Manhattan distance.
    // If Manhattan distance is even, we can reach with parity 0.
    // If odd, we need one extra move (detour), making it even.
    // But we also need diff = 0, not just even total moves.

    // More careful: diff = h_moves - v_moves.
    // Any horizontal move: h_moves += 1, diff += 1
    // Any vertical move: v_moves += 1, diff -= 1
    // To reach target: minimum h_moves >= |sc-tc|, minimum v_moves >= |sr-tr|.
    // diff_min = |sc-tc| - |sr-tr|.
    // We need diff = 0, so we need additional moves:
    // Add extra vertical to reduce diff, or extra horizontal to increase.
    // Extra vertical pair (up+down): v_moves += 2, diff -= 2.
    // Extra horizontal pair (right+left): h_moves += 2, diff += 2.
    // So we can adjust diff by multiples of 2.
    // Need diff_min to be even, i.e., |sc-tc| - |sr-tr| is even.
    // This is equivalent to |sc-tc| + |sr-tr| being even (same parity).

    // If manhattan distance is even, we can achieve diff = 0.
    // If odd, we cannot (need one more move in one direction, changing diff parity).

    // But we also need to verify reachability (no obstacles blocking).

    // BFS from start to check reachability with even Manhattan distance path.
    // State: (r, c, diff mod 2) -- but as shown, any move flips parity.
    // So parity = (number of moves) mod 2... no, parity = diff mod 2.
    // h move: diff mod 2 flips. v move: diff mod 2 flips. So yes, any move flips.
    // Starting diff = 0 (even). After k moves, diff mod 2 = k mod 2.
    // We need diff mod 2 = 0, so even number of moves.
    // And we can adjust diff by 2 via detours.
    // So we need: target reachable via even number of moves.

    // BFS with parity
    vector<vector<array<int,2>>> dist(R, vector<array<int,2>>(C, {-1, -1}));
    queue<tuple<int,int,int>> q;
    dist[sr][sc][0] = 0;
    q.push({sr, sc, 0});

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    while(!q.empty()){
        auto [r, c, p] = q.front(); q.pop();
        int np = 1 - p; // any move flips parity
        for(int d = 0; d < 4; d++){
            int nr = r + dx[d], nc = c + dy[d];
            if(nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if(grid[nr][nc] != '.') continue;
            if(dist[nr][nc][np] != -1) continue;
            dist[nr][nc][np] = dist[r][c][p] + 1;
            q.push({nr, nc, np});
        }
    }

    // Check if target reachable with parity 0
    // Also need at least one cell adjacent to the path for detour
    // (to add horizontal or vertical pair).
    if(dist[tr][tc][0] != -1){
        cout << dist[tr][tc][0] << "\n";
    } else {
        cout << -1 << "\n";
    }

    return 0;
}
