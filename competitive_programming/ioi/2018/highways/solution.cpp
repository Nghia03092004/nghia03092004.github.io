#include <bits/stdc++.h>
using namespace std;

// Grader functions:
long long ask(vector<int> w);
void answer(int s, int t);

namespace {

int m_edges;
int numbered_edges;
long long base_distance;
vector<int> weights;
vector<int> edge_order;
vector<int> edge_child;
vector<vector<pair<int, int>>> adj; // (edge id, neighbor)

int get_first_edge() {
    int lo = 0, hi = m_edges - 1;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        for (int e = 0; e < m_edges; e++) {
            weights[e] = (edge_order[e] <= mid);
        }
        if (ask(weights) != base_distance) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    return lo;
}

int get_last_edge() {
    int lo = -1, hi = numbered_edges - 1;
    while (lo < hi) {
        int mid = (lo + hi + 1) / 2;
        for (int e = 0; e < m_edges; e++) {
            weights[e] = (edge_order[e] >= mid);
        }
        if (ask(weights) != base_distance) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

vector<int> bfs_dist(int root, int n) {
    vector<int> dist(n, -1);
    queue<int> q;
    dist[root] = 0;
    q.push(root);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto [edge_id, to] : adj[v]) {
            if (dist[to] == -1) {
                dist[to] = dist[v] + 1;
                q.push(to);
            }
        }
    }
    return dist;
}

int find_endpoint(int root, int other_root, int critical_edge,
                  const vector<int>& dist_root,
                  const vector<int>& dist_other,
                  int n) {
    edge_order.assign(m_edges, -1);
    edge_child.assign(m_edges, -1);
    numbered_edges = 0;

    vector<int> tree_dist(n, -1);
    queue<int> q;
    tree_dist[root] = 0;
    q.push(root);

    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (auto [edge_id, to] : adj[v]) {
            if (dist_root[to] < dist_other[to]) {
                if (tree_dist[to] == -1) {
                    tree_dist[to] = tree_dist[v] + 1;
                    edge_child[numbered_edges] = to;
                    edge_order[edge_id] = numbered_edges++;
                    q.push(to);
                } else if (tree_dist[to] == tree_dist[v] + 1) {
                    // A shortest-path edge inside the region, but not the chosen BFS-tree edge.
                    edge_order[edge_id] = n;
                }
            } else if (edge_id != critical_edge) {
                // Keep all cross-region edges heavy so shortest paths are forced through the tree.
                edge_order[edge_id] = n;
            }
        }
    }

    int last = get_last_edge();
    return (last == -1 ? root : edge_child[last]);
}

} // namespace

void find_pair(int N, vector<int> U, vector<int> V, int A, int B) {
    (void)A;
    (void)B;

    m_edges = (int)U.size();
    adj.assign(N, {});
    for (int e = 0; e < m_edges; e++) {
        adj[U[e]].push_back({e, V[e]});
        adj[V[e]].push_back({e, U[e]});
    }

    weights.assign(m_edges, 0);
    base_distance = ask(weights);

    edge_order.resize(m_edges);
    iota(edge_order.begin(), edge_order.end(), 0);

    int critical_edge = get_first_edge();
    array<int, 2> roots = {U[critical_edge], V[critical_edge]};

    array<vector<int>, 2> dist = {
        bfs_dist(roots[0], N),
        bfs_dist(roots[1], N)
    };

    int ans0 = find_endpoint(roots[0], roots[1], critical_edge, dist[0], dist[1], N);
    int ans1 = find_endpoint(roots[1], roots[0], critical_edge, dist[1], dist[0], N);
    answer(ans0, ans1);
}

int main() {
    // Grader handles I/O.
    return 0;
}
