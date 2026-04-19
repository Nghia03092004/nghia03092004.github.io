#include <bits/stdc++.h>
using namespace std;

const int MAXC = 200;
const int INF = 1e9;

int R, C;
int H[5001][MAXC]; // H[r][c] = horizontal edge weight in row r, col c to c+1
int V[5001][MAXC]; // V[r][c] = vertical edge weight from (r,c) to (r+1,c)

typedef array<array<int, MAXC>, MAXC> Matrix;

// Compute shortcut matrix for rows [r1, r2)
// D[i][j] = shortest path from (r1, i) to (r2, j)
void computeBlock(int r1, int r2, Matrix &D){
    // Initialize: single row r1
    for(int i = 0; i < C; i++){
        // Start at (r1, i), move horizontally within row r1
        // dist[j] = shortest path from (r1, i) to (r1, j) using row r1 horizontals
        vector<int> dist(C, INF);
        dist[i] = 0;
        for(int j = i + 1; j < C; j++) dist[j] = dist[j-1] + H[r1][j-1];
        for(int j = i - 1; j >= 0; j--) dist[j] = dist[j+1] + H[r1][j];
        for(int j = 0; j < C; j++) D[i][j] = dist[j];
    }

    // Extend row by row
    for(int r = r1; r < r2; r++){
        // Add vertical edges from r to r+1, then horizontal edges in row r+1
        Matrix D2;
        for(int i = 0; i < C; i++){
            // From column i of the top, reaching row r at various columns
            // Now add vertical edge to row r+1
            vector<int> cur(C, INF);
            for(int k = 0; k < C; k++){
                cur[k] = D[i][k] + V[r][k]; // go down from (r, k) to (r+1, k)
            }
            // Now spread horizontally in row r+1
            for(int j = 1; j < C; j++){
                cur[j] = min(cur[j], cur[j-1] + H[r+1][j-1]);
            }
            for(int j = C - 2; j >= 0; j--){
                cur[j] = min(cur[j], cur[j+1] + H[r+1][j]);
            }
            for(int j = 0; j < C; j++) D2[i][j] = cur[j];
        }
        D = D2;
    }
}

// Merge two matrices (min-plus multiplication)
void mergeMatrices(const Matrix &A, const Matrix &B, Matrix &C_out){
    for(int i = 0; i < ::C; i++){
        for(int j = 0; j < ::C; j++){
            C_out[i][j] = INF;
            for(int k = 0; k < ::C; k++){
                C_out[i][j] = min(C_out[i][j], A[i][k] + B[k][j]);
            }
        }
    }
}

// Segment tree on blocks of rows
const int BLOCK_SIZE = 20; // group 20 rows per leaf
const int MAX_LEAVES = 260;
const int MAX_TREE = 1060;

Matrix tree[MAX_TREE];
int nLeaves;

void buildLeaf(int leafIdx){
    int r1 = leafIdx * BLOCK_SIZE;
    int r2 = min(r1 + BLOCK_SIZE, R - 1);
    if(r1 >= R - 1){
        // Identity matrix for empty leaves
        for(int i = 0; i < C; i++)
            for(int j = 0; j < C; j++)
                tree[nLeaves + leafIdx][i][j] = (i == j) ? 0 : INF;
        return;
    }
    computeBlock(r1, r2, tree[nLeaves + leafIdx]);
}

void buildTree(){
    nLeaves = (R - 1 + BLOCK_SIZE - 1) / BLOCK_SIZE;
    // Round up to power of 2
    int sz = 1;
    while(sz < nLeaves) sz *= 2;
    nLeaves = sz;

    // Initialize all leaves
    for(int i = 0; i < nLeaves; i++){
        buildLeaf(i);
    }

    // Build internal nodes
    for(int i = nLeaves - 1; i >= 1; i--){
        mergeMatrices(tree[2*i], tree[2*i+1], tree[i]);
    }
}

void updateLeaf(int leafIdx){
    buildLeaf(leafIdx);
    int idx = nLeaves + leafIdx;
    idx /= 2;
    while(idx >= 1){
        mergeMatrices(tree[2*idx], tree[2*idx+1], tree[idx]);
        idx /= 2;
    }
}

void init(int r, int c, int h[5000][200], int v[5000][200]){
    R = r; C = c;
    for(int i = 0; i < R; i++)
        for(int j = 0; j < C - 1; j++)
            H[i][j] = h[i][j];
    for(int i = 0; i < R - 1; i++)
        for(int j = 0; j < C; j++)
            V[i][j] = v[i][j];
    buildTree();
}

void changeH(int P, int Q, int W){
    H[P][Q] = W;
    int leafIdx = P / BLOCK_SIZE;
    updateLeaf(leafIdx);
}

void changeV(int P, int Q, int W){
    V[P][Q] = W;
    int leafIdx = P / BLOCK_SIZE;
    updateLeaf(leafIdx);
    // Also might affect the next leaf if P is at a block boundary
    if((P + 1) % BLOCK_SIZE == 0 && P + 1 < R - 1){
        updateLeaf(leafIdx + 1);
    }
}

int escape(int V1, int V2){
    return tree[1][V1][V2]; // root of segment tree
}

int main(){
    int r, c;
    cin >> r >> c;
    int h[5000][200], v[5000][200];
    for(int i = 0; i < r; i++)
        for(int j = 0; j < c - 1; j++)
            cin >> h[i][j];
    for(int i = 0; i < r - 1; i++)
        for(int j = 0; j < c; j++)
            cin >> v[i][j];
    init(r, c, h, v);

    int Q;
    cin >> Q;
    while(Q--){
        int type;
        cin >> type;
        if(type == 1){
            int p, q, w;
            cin >> p >> q >> w;
            changeH(p, q, w);
        } else if(type == 2){
            int p, q, w;
            cin >> p >> q >> w;
            changeV(p, q, w);
        } else {
            int v1, v2;
            cin >> v1 >> v2;
            cout << escape(v1, v2) << "\n";
        }
    }
    return 0;
}
