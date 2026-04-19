#include <bits/stdc++.h>
using namespace std;

const int MAXN = 150005;
const int BLOCK = 400;

int N, L;
int pos[MAXN]; // position of elephant i
int nBlocks;
vector<int> blocks[BLOCK + 5]; // each block stores sorted elephant positions

// Jump table for each block:
// For elephant at index j in block b:
// jumps[b][j] = number of cameras from j to end of block
// reach[b][j] = the rightmost point covered when starting from j
int jumps_b[BLOCK + 5][2 * BLOCK + 5];
long long reach_b[BLOCK + 5][2 * BLOCK + 5];

void rebuildBlock(int b){
    int sz = (int)blocks[b].size();
    if(sz == 0) return;

    // Process from right to left
    int j = sz - 1;
    for(int i = sz - 1; i >= 0; i--){
        // Camera starts at blocks[b][i], covers up to blocks[b][i] + L
        long long cover = (long long)blocks[b][i] + L;

        // Find first elephant in this block not covered
        while(j > i && blocks[b][j] > cover) j--;
        // Actually we need the first elephant NOT covered
        // All elephants blocks[b][i..k] with blocks[b][k] <= cover are covered
        // Find k = last covered
        // Binary search for largest index with blocks[b][idx] <= cover
        int lo2 = i, hi2 = sz - 1, k = i;
        while(lo2 <= hi2){
            int mid = (lo2 + hi2) / 2;
            if(blocks[b][mid] <= cover){ k = mid; lo2 = mid + 1; }
            else hi2 = mid - 1;
        }

        if(k == sz - 1){
            // All remaining elephants in block are covered by one camera
            jumps_b[b][i] = 1;
            reach_b[b][i] = cover;
        } else {
            // Need camera for blocks[b][i..k], then continue from k+1
            jumps_b[b][i] = 1 + jumps_b[b][k + 1];
            reach_b[b][i] = reach_b[b][k + 1];
        }
    }
}

void buildAll(){
    // Sort all elephants by position
    vector<pair<int,int>> sorted_pos(N);
    for(int i = 0; i < N; i++) sorted_pos[i] = {pos[i], i};
    sort(sorted_pos.begin(), sorted_pos.end());

    nBlocks = (N + BLOCK - 1) / BLOCK;
    for(int b = 0; b < nBlocks; b++) blocks[b].clear();

    for(int i = 0; i < N; i++){
        int b = i / BLOCK;
        blocks[b].push_back(sorted_pos[i].first);
    }

    for(int b = 0; b < nBlocks; b++){
        rebuildBlock(b);
    }
}

int query(){
    int cameras = 0;
    long long covered = -1e18; // rightmost point covered so far

    for(int b = 0; b < nBlocks; b++){
        if(blocks[b].empty()) continue;
        if(blocks[b].back() <= covered) continue; // whole block covered

        // Find first uncovered elephant in this block
        int idx = (int)(upper_bound(blocks[b].begin(), blocks[b].end(),
                                     (int)covered) - blocks[b].begin());
        if(idx >= (int)blocks[b].size()) continue;

        cameras += jumps_b[b][idx];
        covered = reach_b[b][idx];
    }
    return cameras;
}

void update(int elephantIdx, int newPos){
    int oldPos = pos[elephantIdx];
    pos[elephantIdx] = newPos;

    // Remove oldPos from its block
    for(int b = 0; b < nBlocks; b++){
        auto it = lower_bound(blocks[b].begin(), blocks[b].end(), oldPos);
        if(it != blocks[b].end() && *it == oldPos){
            blocks[b].erase(it);
            rebuildBlock(b);
            break;
        }
    }

    // Insert newPos into correct block
    for(int b = 0; b < nBlocks; b++){
        if(b == nBlocks - 1 || (!blocks[b].empty() && newPos <= blocks[b].back())
           || (b + 1 < nBlocks && !blocks[b+1].empty() && newPos < blocks[b+1][0])){
            auto it = lower_bound(blocks[b].begin(), blocks[b].end(), newPos);
            blocks[b].insert(it, newPos);
            rebuildBlock(b);
            break;
        }
        if(blocks[b].empty()){
            blocks[b].push_back(newPos);
            rebuildBlock(b);
            break;
        }
    }
}

int updateCount = 0;

void init(int n, int l, int positions[]){
    N = n; L = l;
    for(int i = 0; i < N; i++) pos[i] = positions[i];
    buildAll();
    updateCount = 0;
}

int doUpdate(int elephantIdx, int newPos){
    updateCount++;
    if(updateCount % BLOCK == 0) {
        pos[elephantIdx] = newPos;
        buildAll();
    } else {
        update(elephantIdx, newPos);
    }
    return query();
}

int main(){
    int n, l, q;
    cin >> n >> l >> q;
    int *positions = new int[n];
    for(int i = 0; i < n; i++) cin >> positions[i];
    init(n, l, positions);

    // Initial answer
    cout << query() << "\n";

    for(int i = 0; i < q; i++){
        int idx, newp;
        cin >> idx >> newp;
        cout << doUpdate(idx, newp) << "\n";
    }

    delete[] positions;
    return 0;
}
