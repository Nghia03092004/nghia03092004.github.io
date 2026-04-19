#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; // number of points
    cin >> N;

    vector<int> px(N), py(N);
    for(int i = 0; i < N; i++) cin >> px[i] >> py[i];

    int W; // number of walls
    cin >> W;

    vector<int> wa(W), wb(W); // wall endpoints (indices into points)
    for(int i = 0; i < W; i++){
        cin >> wa[i] >> wb[i];
        wa[i]--; wb[i]--; // 0-indexed
    }

    // Coordinate compression
    vector<int> xs, ys;
    for(int i = 0; i < N; i++){
        xs.push_back(px[i]);
        ys.push_back(py[i]);
    }
    sort(xs.begin(), xs.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    auto cx = [&](int x) { return lower_bound(xs.begin(), xs.end(), x) - xs.begin(); };
    auto cy = [&](int y) { return lower_bound(ys.begin(), ys.end(), y) - ys.begin(); };

    int R = ys.size();
    int C = xs.size();

    // Grid of cells: (2*R+1) x (2*C+1) to include gaps between coordinates
    // Cell (2*i+1, 2*j+1) = region between compressed coordinates
    // Cell (2*i, 2*j+1) = horizontal edge
    // Cell (2*i+1, 2*j) = vertical edge
    // Cell (2*i, 2*j) = corner point

    int GR = 2 * R + 1;
    int GC = 2 * C + 1;

    // blocked[r][c] = true if this grid cell is blocked (wall present)
    vector<vector<bool>> blocked(GR, vector<bool>(GC, false));

    // Mark walls on the grid
    for(int i = 0; i < W; i++){
        int x1 = cx(px[wa[i]]), y1 = cy(py[wa[i]]);
        int x2 = cx(px[wb[i]]), y2 = cy(py[wb[i]]);

        // Wall from (x1, y1) to (x2, y2) in compressed coords
        // In the expanded grid: from (2*y1, 2*x1) to (2*y2, 2*x2)

        if(x1 == x2){
            // Vertical wall (same x, different y)
            int mn = min(y1, y2), mx = max(y1, y2);
            int gc = 2 * x1;
            for(int r = 2 * mn; r <= 2 * mx; r++){
                blocked[r][gc] = true;
            }
        } else {
            // Horizontal wall (same y, different x)
            int mn = min(x1, x2), mx = max(x1, x2);
            int gr = 2 * y1;
            for(int c = 2 * mn; c <= 2 * mx; c++){
                blocked[gr][c] = true;
            }
        }
    }

    // BFS from boundary cells to find flooded regions
    // A cell (r, c) with r and c both odd represents a region
    // Edges and corners can be traversed if not blocked

    vector<vector<bool>> visited(GR, vector<bool>(GC, false));
    queue<pair<int,int>> q;

    // Start from all boundary cells that are regions (odd, odd) or edges
    for(int r = 0; r < GR; r++){
        for(int c = 0; c < GC; c++){
            if(r == 0 || r == GR-1 || c == 0 || c == GC-1){
                if(!blocked[r][c] && !visited[r][c]){
                    visited[r][c] = true;
                    q.push({r, c});
                }
            }
        }
    }

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while(!q.empty()){
        auto [r, c] = q.front();
        q.pop();
        for(int d = 0; d < 4; d++){
            int nr = r + dr[d], nc = c + dc[d];
            if(nr < 0 || nr >= GR || nc < 0 || nc >= GC) continue;
            if(visited[nr][nc] || blocked[nr][nc]) continue;
            visited[nr][nc] = true;
            q.push({nr, nc});
        }
    }

    // A wall is visible if it separates a visited (flooded) cell from
    // an unvisited (dry) cell.
    // Check each original wall segment.
    vector<int> result;
    for(int i = 0; i < W; i++){
        int x1 = cx(px[wa[i]]), y1 = cy(py[wa[i]]);
        int x2 = cx(px[wb[i]]), y2 = cy(py[wb[i]]);

        bool visible = false;

        if(x1 == x2){
            // Vertical wall: check cells to left and right
            int mn = min(y1, y2), mx = max(y1, y2);
            int gc = 2 * x1;
            for(int r = 2 * mn + 1; r <= 2 * mx - 1; r += 2){
                // Check cell (r, gc-1) and (r, gc+1)
                bool left = (gc - 1 >= 0) ? visited[r][gc-1] : true;
                bool right = (gc + 1 < GC) ? visited[r][gc+1] : true;
                if(left != right){ visible = true; break; }
            }
        } else {
            // Horizontal wall: check cells above and below
            int mn = min(x1, x2), mx = max(x1, x2);
            int gr = 2 * y1;
            for(int c = 2 * mn + 1; c <= 2 * mx - 1; c += 2){
                bool above = (gr - 1 >= 0) ? visited[gr-1][c] : true;
                bool below = (gr + 1 < GR) ? visited[gr+1][c] : true;
                if(above != below){ visible = true; break; }
            }
        }

        if(visible) result.push_back(i + 1); // 1-indexed
    }

    cout << result.size() << "\n";
    for(int id : result) cout << id << "\n";

    return 0;
}
