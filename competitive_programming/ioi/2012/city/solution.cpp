#include <bits/stdc++.h>
using namespace std;

const int MOD = 1000000000;

// Compute sum of pairwise distances in a tree where each node has weight w[i]
// (number of blocks in the segment). Sum of distances weighted by w.
long long treePairwiseDist(vector<vector<int>> &adj, vector<int> &w, int n){
    if(n == 0) return 0;
    // Root at 0, compute subtree weight sums
    vector<int> subtreeW(n, 0);
    vector<int> parent(n, -1);
    vector<int> order;

    // BFS
    vector<bool> visited(n, false);
    queue<int> q;
    q.push(0); visited[0] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        order.push_back(u);
        for(int v : adj[u]){
            if(!visited[v]){
                visited[v] = true;
                parent[v] = u;
                q.push(v);
            }
        }
    }

    // Compute subtree weights in reverse BFS order
    for(int i = 0; i < n; i++) subtreeW[i] = w[i];
    for(int i = n - 1; i >= 1; i--){
        subtreeW[parent[order[i]]] += subtreeW[order[i]];
    }

    int totalW = subtreeW[0];
    long long result = 0;

    // Each edge (parent[v], v) separates subtreeW[v] and totalW - subtreeW[v]
    for(int i = 1; i < n; i++){
        int v = order[i];
        long long a = subtreeW[v];
        long long b = totalW - a;
        result = (result + a * b) % MOD;
    }

    return result;
}

long long solve(vector<pair<int,int>> &blocks, bool horizontal){
    int N = (int)blocks.size();

    // Sort blocks
    vector<pair<int,int>> sorted_blocks = blocks;
    if(horizontal){
        // Group by row (y), then by x
        sort(sorted_blocks.begin(), sorted_blocks.end(),
             [](const pair<int,int> &a, const pair<int,int> &b){
                 if(a.second != b.second) return a.second < b.second;
                 return a.first < b.first;
             });
    } else {
        // Group by column (x), then by y
        sort(sorted_blocks.begin(), sorted_blocks.end(),
             [](const pair<int,int> &a, const pair<int,int> &b){
                 if(a.first != b.first) return a.first < b.first;
                 return a.second < b.second;
             });
    }

    // Build segments: maximal consecutive runs
    struct Segment {
        int fixedCoord; // row or column
        int start, end; // range in the other coordinate
        int weight;     // number of blocks
    };
    vector<Segment> segments;

    // Assign segment id to each block
    map<pair<int,int>, int> blockToSeg;

    int i = 0;
    while(i < N){
        int j = i;
        if(horizontal){
            int row = sorted_blocks[i].second;
            while(j < N && sorted_blocks[j].second == row &&
                  (j == i || sorted_blocks[j].first == sorted_blocks[j-1].first + 1)){
                j++;
            }
            int segId = (int)segments.size();
            segments.push_back({row, sorted_blocks[i].first, sorted_blocks[j-1].first, j - i});
            for(int k = i; k < j; k++){
                blockToSeg[sorted_blocks[k]] = segId;
            }
        } else {
            int col = sorted_blocks[i].first;
            while(j < N && sorted_blocks[j].first == col &&
                  (j == i || sorted_blocks[j].second == sorted_blocks[j-1].second + 1)){
                j++;
            }
            int segId = (int)segments.size();
            segments.push_back({col, sorted_blocks[i].second, sorted_blocks[j-1].second, j - i});
            for(int k = i; k < j; k++){
                blockToSeg[sorted_blocks[k]] = segId;
            }
        }
        i = j;
    }

    int nSeg = (int)segments.size();

    // Build adjacency between segments
    vector<vector<int>> adj(nSeg);
    set<pair<int,int>> edgeSet;

    // Two segments are adjacent if they are in consecutive rows/columns
    // and share at least one coordinate in the other dimension
    set<pair<int,int>> blockSet(blocks.begin(), blocks.end());

    for(auto &[pos, segId] : blockToSeg){
        pair<int,int> neighbor;
        if(horizontal){
            neighbor = {pos.first, pos.second + 1}; // block above
        } else {
            neighbor = {pos.first + 1, pos.second}; // block to the right
        }
        if(blockToSeg.count(neighbor)){
            int neighborSeg = blockToSeg[neighbor];
            if(neighborSeg != segId){
                auto edge = make_pair(min(segId, neighborSeg), max(segId, neighborSeg));
                if(!edgeSet.count(edge)){
                    edgeSet.insert(edge);
                    adj[segId].push_back(neighborSeg);
                    adj[neighborSeg].push_back(segId);
                }
            }
        }
    }

    // Compute sum of pairwise distances in segment tree
    vector<int> weights(nSeg);
    for(int s = 0; s < nSeg; s++) weights[s] = segments[s].weight;

    return treePairwiseDist(adj, weights, nSeg);
}

int DistanceSum(int N, int *X, int *Y){
    vector<pair<int,int>> blocks(N);
    for(int i = 0; i < N; i++){
        blocks[i] = {X[i], Y[i]};
    }

    long long hDist = solve(blocks, true);  // horizontal segments
    long long vDist = solve(blocks, false); // vertical segments

    return (int)((hDist + vDist) % MOD);
}

int main(){
    int N;
    cin >> N;
    int *X = new int[N], *Y = new int[N];
    for(int i = 0; i < N; i++) cin >> X[i] >> Y[i];
    cout << DistanceSum(N, X, Y) << "\n";
    delete[] X; delete[] Y;
    return 0;
}
