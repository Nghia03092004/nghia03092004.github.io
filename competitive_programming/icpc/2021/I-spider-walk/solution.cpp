#include <algorithm>
#include <cstdint>
#include <deque>
#include <iostream>
#include <set>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

namespace {

struct Bridge {
    int distance;
    int strand;
};

struct Edge {
    int to;
    int cost;
};

void solve() {
    int n, m, s;
    cin >> n >> m >> s;

    vector<Bridge> bridges(m);
    for (int i = 0; i < m; ++i) {
        cin >> bridges[i].distance >> bridges[i].strand;
    }
    sort(bridges.begin(), bridges.end(), [](const Bridge& a, const Bridge& b) {
        return a.distance < b.distance;
    });

    vector<vector<Edge> > reverse_graph;
    auto new_node = [&]() {
        reverse_graph.push_back(vector<Edge>());
        return static_cast<int>(reverse_graph.size()) - 1;
    };
    auto add_directed_edge = [&](int u, int v, int cost) {
        reverse_graph[v].push_back({u, cost});
    };
    auto add_undirected_edge = [&](int u, int v, int cost) {
        add_directed_edge(u, v, cost);
        add_directed_edge(v, u, cost);
    };

    vector<int> start_node(n + 1, -1);
    vector<int> active_node(n + 1, -1);
    vector<int> order(n + 1, 0);

    for (int strand = 1; strand <= n; ++strand) {
        order[strand] = strand;
        int node = new_node();
        start_node[strand] = node;
        active_node[strand] = node;
    }

    auto previous_position = [n](int p) {
        return (p == 1 ? n : p - 1);
    };
    auto next_position = [n](int p) {
        return (p == n ? 1 : p + 1);
    };
    auto add_adjacency = [&](int left_position, int right_position) {
        int left_label = order[left_position];
        int right_label = order[right_position];
        add_undirected_edge(active_node[left_label], active_node[right_label], 1);
    };

    for (int position = 1; position <= n; ++position) {
        add_adjacency(position, next_position(position));
    }

    for (int i = 0; i < m; ++i) {
        int left_position = bridges[i].strand;
        int right_position = next_position(left_position);
        swap(order[left_position], order[right_position]);

        vector<int> refresh_positions;
        refresh_positions.push_back(previous_position(left_position));
        refresh_positions.push_back(left_position);
        refresh_positions.push_back(right_position);
        refresh_positions.push_back(next_position(right_position));
        sort(refresh_positions.begin(), refresh_positions.end());
        refresh_positions.erase(unique(refresh_positions.begin(), refresh_positions.end()),
                                refresh_positions.end());

        for (size_t j = 0; j < refresh_positions.size(); ++j) {
            int position = refresh_positions[j];
            int label = order[position];
            int new_segment = new_node();
            add_directed_edge(active_node[label], new_segment, 0);
            active_node[label] = new_segment;
        }

        set<pair<int, int> > edge_positions;
        for (size_t j = 0; j < refresh_positions.size(); ++j) {
            int position = refresh_positions[j];

            int a = previous_position(position);
            int b = position;
            if (a > b) {
                swap(a, b);
            }
            edge_positions.insert(make_pair(a, b));

            a = position;
            b = next_position(position);
            if (a > b) {
                swap(a, b);
            }
            edge_positions.insert(make_pair(a, b));
        }

        for (set<pair<int, int> >::const_iterator it = edge_positions.begin();
             it != edge_positions.end(); ++it) {
            add_adjacency(it->first, it->second);
        }
    }

    int target_label = order[s];
    int target_node = active_node[target_label];

    const int INF = numeric_limits<int>::max() / 4;
    vector<int> dist(reverse_graph.size(), INF);
    deque<int> dq;
    dist[target_node] = 0;
    dq.push_back(target_node);

    while (!dq.empty()) {
        int u = dq.front();
        dq.pop_front();

        for (size_t i = 0; i < reverse_graph[u].size(); ++i) {
            int v = reverse_graph[u][i].to;
            int w = reverse_graph[u][i].cost;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                if (w == 0) {
                    dq.push_front(v);
                } else {
                    dq.push_back(v);
                }
            }
        }
    }

    for (int strand = 1; strand <= n; ++strand) {
        cout << dist[start_node[strand]] << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
