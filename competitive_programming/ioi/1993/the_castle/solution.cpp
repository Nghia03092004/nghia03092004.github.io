// IOI 1993 - Day 1, Task 1: The Castle
// Flood-fill rooms in a grid with wall bitmasks, then find best wall to remove.
// Wall encoding: bit0=West(1), bit1=North(2), bit2=East(4), bit3=South(8)
#include <bits/stdc++.h>
using namespace std;

int M, N; // M columns, N rows
int grid[55][55];
int roomId[55][55];
int roomSize[2505];
int numRooms;

const int dr[] = {0, -1, 0, 1};  // W, N, E, S
const int dc[] = {-1, 0, 1, 0};
const int wallBit[] = {1, 2, 4, 8};

void bfs(int sr, int sc, int id) {
    queue<pair<int,int>> q;
    q.push({sr, sc});
    roomId[sr][sc] = id;
    int sz = 0;

    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        sz++;
        for (int d = 0; d < 4; d++) {
            if (grid[r][c] & wallBit[d]) continue;
            int nr = r + dr[d], nc = c + dc[d];
            if (nr < 0 || nr >= N || nc < 0 || nc >= M) continue;
            if (roomId[nr][nc] != -1) continue;
            roomId[nr][nc] = id;
            q.push({nr, nc});
        }
    }
    roomSize[id] = sz;
}

int main() {
    scanf("%d%d", &M, &N);

    for (int r = 0; r < N; r++)
        for (int c = 0; c < M; c++)
            scanf("%d", &grid[r][c]);

    // Phase 1: find rooms via flood fill
    memset(roomId, -1, sizeof(roomId));
    numRooms = 0;
    for (int r = 0; r < N; r++)
        for (int c = 0; c < M; c++)
            if (roomId[r][c] == -1)
                bfs(r, c, numRooms++);

    int maxRoom = 0;
    for (int i = 0; i < numRooms; i++)
        maxRoom = max(maxRoom, roomSize[i]);

    printf("%d\n", numRooms);
    printf("%d\n", maxRoom);

    // Phase 2: find best wall to remove
    // Iterate column-by-column (west to east), bottom to top; check N then E
    int bestSize = 0, bestR = -1, bestC = -1;
    char bestDir = ' ';

    for (int c = 0; c < M; c++) {
        for (int r = N - 1; r >= 0; r--) {
            // Check North wall
            if (r > 0 && (grid[r][c] & 2)) {
                int id1 = roomId[r][c], id2 = roomId[r - 1][c];
                if (id1 != id2) {
                    int combined = roomSize[id1] + roomSize[id2];
                    if (combined > bestSize) {
                        bestSize = combined;
                        bestR = r; bestC = c; bestDir = 'N';
                    }
                }
            }
            // Check East wall
            if (c < M - 1 && (grid[r][c] & 4)) {
                int id1 = roomId[r][c], id2 = roomId[r][c + 1];
                if (id1 != id2) {
                    int combined = roomSize[id1] + roomSize[id2];
                    if (combined > bestSize) {
                        bestSize = combined;
                        bestR = r; bestC = c; bestDir = 'E';
                    }
                }
            }
        }
    }

    printf("%d\n", bestSize);
    printf("%d %d %c\n", bestR + 1, bestC + 1, bestDir);
    return 0;
}
