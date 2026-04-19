// IOI 2003 - Amazing (Interactive Maze Navigation)
// Navigate from start to goal in an unknown R x C grid maze.
// Strategy: BFS-based exploration. Maintain a map of visited cells and their
// open directions. At each step, BFS to find path to goal or nearest
// unvisited cell, then take one step along that path.
// Complexity: O(R*C) total moves, O((R*C)^2) worst-case computation.

#include <bits/stdc++.h>
using namespace std;

const int dx[] = {-1, 1, 0, 0};  // N, S, E, W
const int dy[] = {0, 0, 1, -1};
const char dirChar[] = {'N', 'S', 'E', 'W'};

struct Cell {
    int x, y;
    bool operator==(const Cell& o) const { return x == o.x && y == o.y; }
};

int main() {
    int R, C;
    int sr, sc, gr, gc;  // start and goal (1-indexed)
    cin >> R >> C >> sr >> sc >> gr >> gc;

    Cell goal = {gr, gc};
    Cell cur = {sr, sc};

    auto cellKey = [](int x, int y) -> long long {
        return ((long long)x << 32) | (unsigned int)y;
    };

    // Known maze structure
    unordered_map<long long, vector<int>> openDirs;
    unordered_set<long long> visited;

    while (!(cur.x == goal.x && cur.y == goal.y)) {
        long long key = cellKey(cur.x, cur.y);

        // If cell not yet explored, query available directions
        if (!visited.count(key)) {
            visited.insert(key);
            string dirs;
            cin >> dirs;
            vector<int> open;
            for (char c : dirs) {
                for (int d = 0; d < 4; d++) {
                    if (c == dirChar[d]) open.push_back(d);
                }
            }
            openDirs[key] = open;
        }

        // BFS on known graph to find goal or nearest unvisited reachable cell
        queue<Cell> q;
        unordered_map<long long, long long> parent;
        unordered_map<long long, int> parentDir;

        q.push(cur);
        parent[key] = -1;
        Cell target = cur;
        bool foundTarget = false;

        while (!q.empty()) {
            Cell c = q.front();
            q.pop();
            long long ck = cellKey(c.x, c.y);

            if (c.x == goal.x && c.y == goal.y) {
                target = c;
                foundTarget = true;
                break;
            }

            if (!openDirs.count(ck)) continue; // unexplored, can't expand

            for (int d : openDirs[ck]) {
                int nx = c.x + dx[d], ny = c.y + dy[d];
                long long nk = cellKey(nx, ny);
                if (!parent.count(nk)) {
                    parent[nk] = ck;
                    parentDir[nk] = d;
                    q.push({nx, ny});

                    // Prefer goal, but settle for nearest unvisited
                    if (!foundTarget && !visited.count(nk)) {
                        target = {nx, ny};
                        foundTarget = true;
                    }
                }
            }
        }

        // Reconstruct path from cur to target, take first step
        vector<int> path;
        long long tk = cellKey(target.x, target.y);
        while (tk != cellKey(cur.x, cur.y)) {
            path.push_back(parentDir[tk]);
            tk = parent[tk];
        }
        reverse(path.begin(), path.end());

        if (!path.empty()) {
            int d = path[0];
            cout << dirChar[d] << endl;
            cur.x += dx[d];
            cur.y += dy[d];
        }
    }

    return 0;
}
