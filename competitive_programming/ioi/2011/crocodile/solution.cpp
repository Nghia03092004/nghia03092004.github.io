#include <bits/stdc++.h>
using namespace std;

int travel_plan(int N, int M, int R[][2], int L[], int K, int P[]){
    vector<vector<pair<int,int>>> adj(N);
    for(int i = 0; i < M; i++){
        adj[R[i][0]].push_back({R[i][1], L[i]});
        adj[R[i][1]].push_back({R[i][0], L[i]});
    }

    const long long INF = 1e18;
    // d[v][0] = best distance, d[v][1] = second best distance
    vector<array<long long, 2>> d(N, {INF, INF});

    // Min-heap: (distance, node) -- we push when d2 is set
    priority_queue<pair<long long,int>, vector<pair<long long,int>>,
                   greater<pair<long long,int>>> pq;

    for(int i = 0; i < K; i++){
        d[P[i]][0] = d[P[i]][1] = 0;
        pq.push({0, P[i]});
    }

    vector<bool> done(N, false);

    while(!pq.empty()){
        auto [dist, u] = pq.top(); pq.pop();
        if(done[u]) continue;
        if(dist > d[u][1]) continue;
        done[u] = true;

        for(auto [v, w] : adj[u]){
            long long nd = d[u][1] + w; // use second-best from u
            if(nd < d[v][0]){
                d[v][1] = d[v][0];
                d[v][0] = nd;
            } else if(nd < d[v][1]){
                d[v][1] = nd;
            } else {
                continue;
            }
            if(d[v][1] < INF){
                pq.push({d[v][1], v});
            }
        }
    }

    return (int)d[0][1];
}

// Standalone main for testing
int main(){
    int N, M, K;
    cin >> N >> M >> K;

    // Read edges
    int (*R)[2] = new int[M][2];
    int *L = new int[M];
    for(int i = 0; i < M; i++){
        cin >> R[i][0] >> R[i][1] >> L[i];
    }
    int *P = new int[K];
    for(int i = 0; i < K; i++) cin >> P[i];

    cout << travel_plan(N, M, R, L, K, P) << "\n";

    delete[] R;
    delete[] L;
    delete[] P;
    return 0;
}
