#include <bits/stdc++.h>
using namespace std;

const int INF = 1e9;

int N, K;
vector<pair<int,int>> adj[200005]; // (neighbor, weight)
int subtreeSize[200005];
bool removed[200005];
int ans;

int best_lookup[1000005]; // best[dist] = min edges to reach dist from centroid

int getSubtreeSize(int u, int parent){
    subtreeSize[u] = 1;
    for(auto [v, w] : adj[u]){
        if(v != parent && !removed[v]){
            subtreeSize[u] += getSubtreeSize(v, u);
        }
    }
    return subtreeSize[u];
}

int getCentroid(int u, int parent, int treeSize){
    for(auto [v, w] : adj[u]){
        if(v != parent && !removed[v]){
            if(subtreeSize[v] > treeSize / 2)
                return getCentroid(v, u, treeSize);
        }
    }
    return u;
}

// Collect all (distance, depth) pairs in subtree
vector<pair<long long, int>> collected;

void dfs(int u, int parent, long long dist, int depth){
    if(dist > K) return;
    collected.push_back({dist, depth});
    for(auto [v, w] : adj[u]){
        if(v != parent && !removed[v]){
            dfs(v, u, dist + w, depth + 1);
        }
    }
}

// Track which distances we've written to best_lookup for cleanup
vector<long long> toClean;

void solve(int u){
    int sz = getSubtreeSize(u, -1);
    int centroid = getCentroid(u, -1, sz);
    removed[centroid] = true;

    // Process paths through centroid
    // best_lookup[d] = min edges for distance d from centroid (from previous subtrees)
    best_lookup[0] = 0; // distance 0 from centroid = 0 edges
    toClean.push_back(0);

    for(auto [v, w] : adj[centroid]){
        if(removed[v]) continue;

        collected.clear();
        dfs(v, centroid, w, 1);

        // Check against best_lookup
        for(auto [dist, depth] : collected){
            long long need = K - dist;
            if(need >= 0 && need <= K && best_lookup[need] < INF){
                ans = min(ans, depth + best_lookup[need]);
            }
        }

        // Update best_lookup with this subtree
        for(auto [dist, depth] : collected){
            if(dist <= K){
                if(best_lookup[dist] > depth){
                    best_lookup[dist] = depth;
                    toClean.push_back(dist);
                }
            }
        }
    }

    // Cleanup
    for(long long d : toClean){
        best_lookup[d] = INF;
    }
    toClean.clear();

    // Recurse
    for(auto [v, w] : adj[centroid]){
        if(!removed[v]){
            solve(v);
        }
    }
}

int best_distance(int n, int k, int edges[][3]){
    N = n; K = k;
    ans = INF;

    for(int i = 0; i < N; i++) adj[i].clear();
    memset(removed, false, sizeof(removed));
    fill(best_lookup, best_lookup + K + 1, INF);

    for(int i = 0; i < N - 1; i++){
        int u = edges[i][0], v = edges[i][1], w = edges[i][2];
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }

    solve(0);

    return (ans >= INF) ? -1 : ans;
}

int main(){
    int n, k;
    cin >> n >> k;

    int (*edges)[3] = new int[n-1][3];
    for(int i = 0; i < n - 1; i++){
        cin >> edges[i][0] >> edges[i][1] >> edges[i][2];
    }

    cout << best_distance(n, k, edges) << "\n";
    delete[] edges;
    return 0;
}
