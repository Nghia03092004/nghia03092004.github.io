#include <bits/stdc++.h>
using namespace std;

// Problem 300: Protein Folding
//
// For each of 2^15 protein strings of length 15 (H=0, P=1 in bitmask),
// find the optimal (max) number of H-H contacts over all SAWs.
// H-H contact: two H residues at lattice-adjacent positions.
// This includes BOTH consecutive and non-consecutive pairs.
//
// Consecutive pairs (i, i+1) are ALWAYS lattice-adjacent in any SAW,
// so their contribution is walk-independent and can be precomputed.
//
// Non-consecutive contacts (|i-j| > 1) depend on the walk.
// We enumerate all SAWs, find the maximum non-consecutive contacts per protein,
// then add the consecutive contacts.

const int N = 15;
const int GRID = 31;
const int CENTER = 15;

int pos[N][2];
bool visited[GRID][GRID];
int best[1 << N]; // best non-consecutive contacts per protein

set<vector<int>> seen_topologies;

void generateWalks(int step) {
    if (step == N) {
        // Find non-consecutive contact pairs
        vector<int> contact_masks;
        for (int i = 0; i < N; i++) {
            for (int j = i + 2; j < N; j++) {
                int dx = abs(pos[i][0] - pos[j][0]);
                int dy = abs(pos[i][1] - pos[j][1]);
                if (dx + dy == 1) {
                    contact_masks.push_back((1 << i) | (1 << j));
                }
            }
        }
        if (contact_masks.empty()) return;

        sort(contact_masks.begin(), contact_masks.end());
        if (!seen_topologies.insert(contact_masks).second) return;

        int nc = contact_masks.size();
        for (int protein = 0; protein < (1 << N); protein++) {
            int c = 0;
            for (int k = 0; k < nc; k++) {
                if ((protein & contact_masks[k]) == 0) c++;
            }
            if (c > best[protein]) best[protein] = c;
        }
        return;
    }

    static const int dx[] = {1, -1, 0, 0};
    static const int dy[] = {0, 0, 1, -1};
    for (int dir = 0; dir < 4; dir++) {
        int nx = pos[step-1][0] + dx[dir];
        int ny = pos[step-1][1] + dy[dir];
        if (nx < 0 || nx >= GRID || ny < 0 || ny >= GRID) continue;
        if (visited[nx][ny]) continue;
        pos[step][0] = nx;
        pos[step][1] = ny;
        visited[nx][ny] = true;
        generateWalks(step + 1);
        visited[nx][ny] = false;
    }
}

int main(){
    memset(best, 0, sizeof(best));
    memset(visited, false, sizeof(visited));

    pos[0][0] = CENTER;
    pos[0][1] = CENTER;
    visited[CENTER][CENTER] = true;

    // Enumerate all SAWs (all 4 starting directions)
    generateWalks(1);

    // Compute total: best non-consecutive contacts + consecutive H-H contacts
    long long total = 0;
    for (int protein = 0; protein < (1 << N); protein++) {
        int consec = 0;
        for (int i = 0; i < N - 1; i++) {
            if (((protein >> i) & 1) == 0 && ((protein >> (i+1)) & 1) == 0) {
                consec++;
            }
        }
        total += best[protein] + consec;
    }

    double avg = (double)total / (1 << N);
    printf("%.13f\n", avg);

    return 0;
}
