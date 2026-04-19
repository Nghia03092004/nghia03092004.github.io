#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

struct DSU {
    vector<int> p, rank_;
    void init(int n) {
        p.resize(n); rank_.resize(n, 0);
        iota(p.begin(), p.end(), 0);
    }
    int find(int x) { return p[x] == x ? x : p[x] = find(p[x]); }
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) swap(a, b);
        p[b] = a;
        if (rank_[a] == rank_[b]) rank_[a]++;
        return true;
    }
};

long long plan_roller_coaster(int n, int s[], int t[]) {
    // Coordinate compress all speeds
    vector<int> vals;
    for (int i = 0; i < n; i++) {
        vals.push_back(s[i]);
        vals.push_back(t[i]);
    }
    // Add a "zero" node for the circuit starting point
    vals.push_back(0);
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int V = vals.size();

    auto compress = [&](int x) {
        return lower_bound(vals.begin(), vals.end(), x) - vals.begin();
    };

    // Compute degree balance at each speed node
    // Each section i: edge from s[i] to t[i]
    // in-degree of t[i] += 1, out-degree of s[i] += 1
    vector<int> balance(V, 0); // balance = out - in
    DSU dsu;
    dsu.init(V);

    for (int i = 0; i < n; i++) {
        int u = compress(s[i]), v = compress(t[i]);
        balance[u]++; // out-degree
        balance[v]--; // in-degree
        dsu.unite(u, v);
    }

    // The circuit needs to return to start.
    // Add a "base" node at speed 0. The circuit starts and ends here.
    // Actually, for the Eulerian circuit, we need the total system to be balanced.
    // Add one edge from node 0 to some node, and back.
    // The circuit: start at speed 0, ride sections, return to speed 0.
    // So we need an edge from 0 -> s[first] and t[last] -> 0.
    // For Eulerian: just ensure balance at each node and connectivity.

    // For the circuit: add one outgoing from node 0 and one incoming to node 0
    // But this is implicit in the circuit structure. Actually the problem says
    // arrange sections into a circuit, so we need an Eulerian circuit on the
    // section edges plus any added "transition" edges.

    // The trick: we need to add edges to balance degrees and connect components.
    // Sweep from highest speed to lowest:
    // - Accumulate excess = sum of balance[v] for v >= current
    // - If excess < 0 at some point: we need |excess| edges going "up" from
    //   current speed to the next higher speed, costing (next - current) each.
    // - Also need connectivity.

    // First, connect components for connectivity
    // All nodes with edges are in the DSU. Also include node 0.
    // Connect all components: sort by value, connect adjacent with cost diff.

    // Which nodes are "active"? Those with edges or non-zero balance.
    vector<bool> active(V, false);
    active[compress(0)] = true; // base node
    for (int i = 0; i < n; i++) {
        active[compress(s[i])] = true;
        active[compress(t[i])] = true;
    }

    // For connectivity: connect adjacent active nodes
    // Cost of connecting nodes at speeds a < b: b - a (need an up-edge)
    // But actually, connecting down is free.
    // For Eulerian circuit on an undirected-like structure, we just need connectivity.
    // The cost to connect two components at speeds a and b (a < b) is b - a.

    // Collect active node indices sorted by value
    vector<int> activeNodes;
    for (int i = 0; i < V; i++) {
        if (active[i]) activeNodes.push_back(i);
    }

    ll cost = 0;

    // Connect components using edges between adjacent active nodes
    for (int i = 0; i + 1 < (int)activeNodes.size(); i++) {
        int u = activeNodes[i], v = activeNodes[i+1];
        if (dsu.unite(u, v)) {
            // Must add an edge from u to v (cost = vals[v] - vals[u])
            // and an edge from v to u (cost = 0, since going down is free)
            cost += vals[v] - vals[u];
            balance[u]++; // outgoing from u
            balance[v]--; // incoming to v
            // Also add return edge (free)
            balance[v]++; // outgoing from v
            balance[u]--; // incoming to u
        }
    }

    // Now balance degrees using sweep from right to left
    // At each active node, accumulate excess. If excess > 0, we have more
    // outgoing than incoming, need to send excess to the right (but that costs
    // money going up). If excess < 0, we have more incoming, need to send
    // excess to the left (free going down).
    // Sweep from right to left, carry excess.
    // Cost is incurred when excess > 0 at a transition going right.

    // Actually, sweep from left to right:
    // carry = running sum of balance
    // At each step, if carry > 0: we need 'carry' transitions going left (free)
    // If carry < 0: we need |carry| transitions going right (cost per unit = gap)
    // Wait, this needs more care.

    // Correct approach: sweep left to right on active nodes.
    // carry tracks the net flow that needs to go right.
    // carry = sum of balance[0..i]. If carry > 0, we have carry units of flow
    // going right from node i to node i+1, cost = carry * (vals[i+1] - vals[i]).
    // If carry < 0, the flow goes left (free).

    ll carry = 0;
    for (int i = 0; i + 1 < (int)activeNodes.size(); i++) {
        carry += balance[activeNodes[i]];
        if (carry > 0) {
            cost += carry * (ll)(vals[activeNodes[i+1]] - vals[activeNodes[i]]);
        }
    }

    return cost;
}

int main() {
    int n;
    scanf("%d", &n);
    int s[n], t[n];
    for (int i = 0; i < n; i++) scanf("%d %d", &s[i], &t[i]);
    printf("%lld\n", plan_roller_coaster(n, s, t));
    return 0;
}
