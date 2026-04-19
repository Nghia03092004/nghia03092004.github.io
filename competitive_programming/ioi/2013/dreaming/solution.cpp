#include <bits/stdc++.h>
using namespace std;

int travelTime(int N, int M, int L, int A[], int B[], int T[]){
    vector<vector<pair<int,int>>> adj(N);
    for(int i = 0; i < M; i++){
        adj[A[i]].push_back({B[i], T[i]});
        adj[B[i]].push_back({A[i], T[i]});
    }

    vector<bool> visited(N, false);
    vector<long long> dist(N, 0);

    // BFS/DFS to find farthest node from a given start
    auto bfs = [&](int start) -> pair<int, long long> {
        // Returns (farthest node, distance to it)
        queue<int> q;
        q.push(start);
        fill(dist.begin(), dist.end(), -1);
        dist[start] = 0;
        int farthest = start;
        long long maxDist = 0;
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(auto [v, w] : adj[u]){
                if(dist[v] == -1){
                    dist[v] = dist[u] + w;
                    if(dist[v] > maxDist){
                        maxDist = dist[v];
                        farthest = v;
                    }
                    q.push(v);
                }
            }
        }
        return {farthest, maxDist};
    };

    // Find connected components, compute diameter and radius for each
    vector<long long> radii;
    long long maxDiam = 0;

    for(int s = 0; s < N; s++){
        if(visited[s]) continue;

        // Find all nodes in this component
        // BFS from s
        auto [u, d1] = bfs(s);
        // u is farthest from s. BFS from u to find diameter endpoint.
        auto [v, diameter] = bfs(u);
        maxDiam = max(maxDiam, diameter);

        // Mark all visited
        // Also find radius: BFS from v, get dist from v.
        // The diameter path is u-v. Find center of this path.
        // Radius = ceil(diameter / 2) -- but with weighted edges, it's more nuanced.
        // Radius = min over all nodes x of max(dist_from_u[x], dist_from_v[x])

        // dist currently holds distances from u.
        vector<long long> distU(N, -1);
        // Actually dist was set by bfs(u). Copy relevant values.
        // Let me redo: BFS from u, save distances. BFS from v, save distances.
        bfs(u);
        vector<long long> dU(N);
        for(int i = 0; i < N; i++) dU[i] = dist[i];

        bfs(v);
        vector<long long> dV(N);
        for(int i = 0; i < N; i++) dV[i] = dist[i];

        // Find radius: for each node in component, eccentricity = max(dU[x], dV[x])
        // (since u and v are diameter endpoints, max distance from any node is
        // max(dist to u, dist to v))
        // Radius = min eccentricity over all nodes in component.
        long long radius = LLONG_MAX;
        for(int x = 0; x < N; x++){
            if(dU[x] >= 0 && dV[x] >= 0){
                long long ecc = max(dU[x], dV[x]);
                radius = min(radius, ecc);
                visited[x] = true;
            }
        }

        radii.push_back(radius);
    }

    sort(radii.rbegin(), radii.rend()); // decreasing

    long long ans = maxDiam;

    if(radii.size() >= 2){
        ans = max(ans, radii[0] + radii[1] + L);
    }
    if(radii.size() >= 3){
        ans = max(ans, radii[1] + radii[2] + 2LL * L);
    }

    return (int)ans;
}

int main(){
    int N, M, L;
    cin >> N >> M >> L;
    int *A = new int[M], *B = new int[M], *T = new int[M];
    for(int i = 0; i < M; i++){
        cin >> A[i] >> B[i] >> T[i];
    }
    cout << travelTime(N, M, L, A, B, T) << "\n";
    delete[] A; delete[] B; delete[] T;
    return 0;
}
