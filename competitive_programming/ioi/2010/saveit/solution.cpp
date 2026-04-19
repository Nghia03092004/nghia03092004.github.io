// IOI 2010 - Saveit
// Encode/decode shortest-path distances via BFS spanning tree + base-3 diffs.
// Bits: O(N log N + H * N).
#include <bits/stdc++.h>
using namespace std;

// Grader-provided bit I/O.
void encode_bit(int b);
int decode_bit();

// ===== ENCODER =====
void encode(int N, int H, int M, vector<pair<int, int>> &edges) {
    vector<vector<int>> adj(N);
    for (auto &[u, v] : edges) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // BFS from node 0 to build spanning tree.
    vector<int> parent(N, -1);
    vector<int> bfsOrder;
    vector<bool> visited(N, false);
    queue<int> q;
    q.push(0); visited[0] = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        bfsOrder.push_back(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    // Encode parent array: 10 bits per node (N <= 1000).
    const int BITS = 10;
    for (int i = 1; i < N; i++) {
        for (int b = BITS - 1; b >= 0; b--) {
            encode_bit((parent[i] >> b) & 1);
        }
    }

    // BFS from each hub to get all distances.
    vector<vector<int>> dist(H, vector<int>(N, -1));
    for (int h = 0; h < H; h++) {
        queue<int> bfs;
        bfs.push(h); dist[h][h] = 0;
        while (!bfs.empty()) {
            int u = bfs.front(); bfs.pop();
            for (int v : adj[u]) {
                if (dist[h][v] == -1) {
                    dist[h][v] = dist[h][u] + 1;
                    bfs.push(v);
                }
            }
        }
    }

    // Encode distance differences along BFS tree edges.
    // delta = dist[h][v] - dist[h][parent[v]] + 1, values in {0, 1, 2}.
    // Pack two base-3 values into 4 bits (0..8 < 16).
    vector<int> deltas;
    for (int h = 0; h < H; h++) {
        for (int i = 1; i < N; i++) {
            int v = bfsOrder[i];
            int p = parent[v];
            int d = dist[h][v] - dist[h][p] + 1;
            deltas.push_back(d);
        }
    }
    if (deltas.size() % 2 != 0) deltas.push_back(0);

    for (int i = 0; i < (int)deltas.size(); i += 2) {
        int val = deltas[i] * 3 + deltas[i + 1];
        for (int b = 3; b >= 0; b--) {
            encode_bit((val >> b) & 1);
        }
    }
}

// ===== DECODER =====
void decode(int N, int H) {
    const int BITS = 10;

    // Read parent array.
    vector<int> parent(N, -1);
    for (int i = 1; i < N; i++) {
        int p = 0;
        for (int b = BITS - 1; b >= 0; b--) {
            p |= (decode_bit() << b);
        }
        parent[i] = p;
    }

    // Reconstruct BFS order from tree.
    vector<vector<int>> children(N);
    for (int i = 1; i < N; i++) children[parent[i]].push_back(i);

    vector<int> bfsOrder;
    queue<int> q;
    q.push(0);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        bfsOrder.push_back(u);
        for (int c : children[u]) q.push(c);
    }

    // Read packed deltas.
    int totalDeltas = H * (N - 1);
    int paddedSize = (totalDeltas + 1) / 2 * 2;
    vector<int> deltas;
    for (int i = 0; i < paddedSize; i += 2) {
        int val = 0;
        for (int b = 3; b >= 0; b--) {
            val |= (decode_bit() << b);
        }
        deltas.push_back(val / 3);
        deltas.push_back(val % 3);
    }

    // Reconstruct distances.
    vector<vector<int>> dist(H, vector<int>(N, 0));
    int idx = 0;
    for (int h = 0; h < H; h++) {
        for (int i = 1; i < N; i++) {
            int v = bfsOrder[i];
            int p = parent[v];
            int d = deltas[idx++] - 1; // map {0,1,2} back to {-1,0,+1}
            dist[h][v] = dist[h][p] + d;
        }
    }

    // Output distances.
    for (int h = 0; h < H; h++) {
        for (int v = 0; v < N; v++) {
            cout << dist[h][v];
            if (v + 1 < N) cout << " ";
        }
        cout << "\n";
    }
}

// Stub main; in contest, grader calls encode/decode directly.
int main() {
    return 0;
}

void encode_bit(int /*b*/) {}
int decode_bit() { return 0; }
