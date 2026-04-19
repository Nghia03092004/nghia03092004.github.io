#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, P, Q;
    cin >> N >> M >> P >> Q;

    // Edges sorted by beauty (index 0 = most beautiful)
    // adj[v] = list of (beauty_rank, neighbor, edge_index)
    // For each node, we only care about the top-2 edges (smallest indices = most beautiful)
    vector<vector<pair<int,int>>> adj(N); // adj[v] = sorted list of (edge_idx, neighbor)

    vector<pair<int,int>> edges(M);
    for(int i = 0; i < M; i++){
        int u, v;
        cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back({i, v});
        adj[v].push_back({i, u});
    }

    // Sort adjacency lists by edge index (smaller = more beautiful)
    for(int v = 0; v < N; v++){
        sort(adj[v].begin(), adj[v].end());
    }

    // For each node v, best[v] = the neighbor via the most beautiful edge
    // second[v] = the neighbor via the second most beautiful edge (if exists)
    // State: (v, came_from_best)
    // If came_from_best=1, we entered v via best edge, so we leave via second
    // If came_from_best=0, we entered v via another edge (or start), leave via best

    // next[v][0] = next state if we came NOT from best edge -> go via best
    // next[v][1] = next state if we came from best edge -> go via second

    // Total states: 2N
    // State id: v*2 + flag, where flag=0 means "go via best", flag=1 means "go via second"

    int S = 2 * N;
    vector<int> nxt(S, -1); // next state

    for(int v = 0; v < N; v++){
        if(adj[v].empty()) continue;

        int best_neighbor = adj[v][0].second;
        int best_edge = adj[v][0].first;

        int second_neighbor = -1;
        if((int)adj[v].size() >= 2){
            second_neighbor = adj[v][1].second;
        }

        // State v*2+0: go via best edge to best_neighbor
        // At best_neighbor, did we come from best_neighbor's best edge?
        // We came via edge best_edge. best_neighbor's best edge is adj[best_neighbor][0].first
        if(best_neighbor >= 0){
            int arrived_via_best = (adj[best_neighbor][0].first == best_edge) ? 1 : 0;
            nxt[v*2+0] = best_neighbor * 2 + arrived_via_best;
        }

        // State v*2+1: go via second edge
        if(second_neighbor >= 0){
            int sec_edge = adj[v][1].first;
            int arrived_via_best = (adj[second_neighbor][0].first == sec_edge) ? 1 : 0;
            nxt[v*2+1] = second_neighbor * 2 + arrived_via_best;
        } else {
            // No second edge, must go via best even if we came from there
            if(best_neighbor >= 0){
                int arrived_via_best = (adj[best_neighbor][0].first == best_edge) ? 1 : 0;
                nxt[v*2+1] = best_neighbor * 2 + arrived_via_best;
            }
        }
    }

    // Target states: P*2+0 and P*2+1
    // For each query K, count starting nodes s such that after K steps from
    // state s*2+0 (starting means we came from "not best", so we go via best),
    // we reach P*2+0 or P*2+1.

    // Actually, starting from node s, the initial state depends on the first move.
    // At the start, there's no "previous edge", so we take the best edge.
    // Initial state for node s is s*2+0 (go via best).

    // For the functional graph, find cycle structure.
    // For each of the two target states t in {P*2+0, P*2+1}:
    //   Walk forward from t to find the cycle.
    //   Then for each starting state s*2+0, determine if it reaches t in K steps.

    // Approach: for target state t, compute dist[s] = minimum steps from s to t.
    // In a functional graph, use reverse graph: build tree of predecessors.

    // Build reverse graph
    vector<vector<int>> rev(S);
    for(int s = 0; s < S; s++){
        if(nxt[s] >= 0 && nxt[s] < S){
            rev[nxt[s]].push_back(s);
        }
    }

    // For each target state, find cycle length and tail distances
    // Then answer queries.

    // Find cycle containing each target
    auto findCycle = [&](int start) -> pair<int,int> {
        // Returns (cycle_length, distance_from_start_to_cycle_entry)
        // Using Floyd's algorithm
        int slow = start, fast = start;
        do {
            slow = nxt[slow];
            fast = nxt[nxt[fast]];
        } while(slow != fast);

        // Find cycle entry
        slow = start;
        int dist = 0;
        while(slow != fast){
            slow = nxt[slow];
            fast = nxt[fast];
            dist++;
        }
        int entry = slow;

        // Find cycle length
        int len = 1;
        int cur = nxt[entry];
        while(cur != entry){ cur = nxt[cur]; len++; }

        return {len, dist};
    };

    // For each target t, BFS backwards to find all nodes at distance exactly K
    // But K can be up to 10^18, so we can't BFS K steps.
    // Instead: find the cycle, and for tail nodes check exact distance,
    // for cycle nodes check distance mod cycle_length.

    // Let me find, for each of the two target states:
    // - The distance from each state to the target (if on the same rho-path)
    // - The cycle length

    // Simpler approach: for target t, walk forward to detect cycle.
    // All states on the rho that eventually reach t's cycle:
    //   - States on the tail at distance d from t: reach t in d steps (if d <= K)
    //   - States on the cycle: reach t in d steps where d ≡ (K mod C)

    // But we need REVERSE: which states reach t in K steps.
    // Walk backward from t, level by level, for levels 0, 1, 2, ...
    // Nodes on the reverse tree at level K reach t in K steps.
    // But K can be huge, so we walk until we hit the cycle, then use modular arithmetic.

    auto solve = [&](int target, int K) -> int {
        // Find all states s*2+0 (starting states) that reach target in K steps.
        // Reverse BFS from target.
        // Detect cycle in forward graph containing target.

        // Walk forward from target to find cycle
        vector<int> path;
        set<int> visited;
        int cur = target;
        while(!visited.count(cur)){
            visited.insert(cur);
            path.push_back(cur);
            cur = nxt[cur];
        }
        // cur is the cycle entry, path goes target -> ... -> cur (cycle entry)
        // Then cycle goes cur -> ... -> cur

        int cycleEntry = cur;
        int tailLen = 0;
        for(int i = 0; i < (int)path.size(); i++){
            if(path[i] == cycleEntry){ tailLen = i; break; }
        }

        // Find cycle length
        int cycleLen = 1;
        int c = nxt[cycleEntry];
        while(c != cycleEntry){ c = nxt[c]; cycleLen++; }

        // Now BFS backward from target.
        // Nodes at distance d from target (going backward) can reach target in d steps.
        // For nodes NOT on the cycle: they have a unique distance.
        // For nodes ON the cycle: if distance d works, then d + k*cycleLen also works.

        // We BFS backward from target, but we must be careful about the cycle.
        // Nodes on the cycle from target are at distances 0, cycleLen, 2*cycleLen, ...

        // Strategy: BFS backward, but stop at depth min(K, 2N) for tail nodes.
        // For cycle nodes, check if K ≡ d (mod cycleLen).

        // Identify cycle nodes
        set<int> onCycle;
        onCycle.insert(cycleEntry);
        c = nxt[cycleEntry];
        while(c != cycleEntry){ onCycle.insert(c); c = nxt[c]; }

        // BFS backward from target, track distances
        // For cycle: the target itself is on the cycle (or on the tail).
        // If target is on the cycle, then all cycle nodes can reach target.
        // A cycle node at "cycle distance" d from target is reachable in d, d+C, d+2C, ...

        int count = 0;

        // Check if target is on the cycle
        bool targetOnCycle = onCycle.count(target);

        if(targetOnCycle){
            // All cycle nodes at cycle-distance d from target where K ≡ d (mod C)
            // Walk around cycle backward
            // But also need to consider tail nodes reaching into cycle nodes.

            // Cycle nodes reaching target at distance d (0, 1, ..., C-1):
            vector<int> cycleNodes;
            cycleNodes.push_back(target); // distance 0
            // Walk backward on cycle
            // Need reverse on cycle
            // For each cycle node, its predecessor on the cycle is the one
            // that maps to it via nxt.
            // Already have rev graph, but rev may have multiple predecessors.
            // On the cycle, each node has exactly one predecessor ON the cycle.

            // Let's just collect cycle nodes with their distance from target
            map<int, long long> cycleDist; // state -> distance from target (going backward on cycle)
            cycleDist[target] = 0;
            // Walk cycle backward: find predecessor of target on cycle
            int prev = -1;
            for(int r : rev[target]){
                if(onCycle.count(r)){ prev = r; break; }
            }
            long long d = 1;
            int node = prev;
            while(node != target){
                cycleDist[node] = d;
                d++;
                for(int r : rev[node]){
                    if(onCycle.count(r)){ node = r; break; }
                }
            }

            // For each cycle node, check if any starting state reaches it
            // at the right time.
            // A starting state s reaches cycle node cn at distance
            // (dist_from_s_to_cn) steps. Then from cn, it takes
            // cycleDist[cn] more steps (possibly + multiples of C) to reach target.
            // Total = dist_from_s_to_cn + cycleDist[cn] + k*C = K for some k >= 0.

            // For each cycle node cn, BFS backward through NON-cycle nodes (tail trees).
            for(auto &[cn, cdist] : cycleDist){
                // Need total distance = K, and total distance >= cdist
                // tail_dist + cdist ≡ K (mod C) and tail_dist + cdist <= K
                // tail_dist ≡ K - cdist (mod C)
                long long need = ((K - cdist) % cycleLen + cycleLen) % cycleLen;
                if(need + cdist > K && need > 0) continue; // even smallest doesn't fit
                if(cdist > K) continue;

                // BFS backward from cn through non-cycle edges
                // Collect all non-cycle predecessors at distance need, need+C, need+2C, ...
                // Since tail is a tree, distances are unique.
                // BFS:
                queue<pair<int, long long>> bfs;
                bfs.push({cn, 0});
                while(!bfs.empty()){
                    auto [u, dd] = bfs.front(); bfs.pop();
                    // dd = distance from cn going backward
                    long long totalDist = dd + cdist;
                    if(totalDist > K) continue;
                    if(totalDist >= 0 && (K - totalDist) % cycleLen == 0){
                        // State u reaches target in K steps
                        if(u % 2 == 0){
                            // This is a starting state (node u/2)
                            count++;
                        }
                    }
                    for(int r : rev[u]){
                        if(!onCycle.count(r)){
                            bfs.push({r, dd + 1});
                        }
                    }
                }
            }
        } else {
            // Target is on the tail, not on cycle.
            // Only nodes on the reverse tree from target can reach it.
            // BFS backward from target.
            queue<pair<int, long long>> bfs;
            bfs.push({target, 0});
            while(!bfs.empty()){
                auto [u, dd] = bfs.front(); bfs.pop();
                if(dd == K){
                    if(u % 2 == 0) count++;
                    continue; // don't go deeper
                }
                if(dd > K) continue;
                for(int r : rev[u]){
                    bfs.push({r, dd + 1});
                }
            }
        }

        return count;
    };

    while(Q--){
        long long K;
        cin >> K;
        // Count starting nodes that reach P in K steps
        // Starting state for node s is s*2+0
        // Target: we need to reach state P*2+0 or P*2+1
        int ans = 0;
        // We should combine both targets, but avoid double-counting
        // A starting node reaching both targets in K steps shouldn't be counted twice.
        // Actually, each starting state has a unique path, so it reaches at most one state.
        // So we can just add.
        ans += solve(P * 2 + 0, K);
        ans += solve(P * 2 + 1, K);
        cout << ans << "\n";
    }

    return 0;
}
