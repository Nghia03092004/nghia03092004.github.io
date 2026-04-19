#include <bits/stdc++.h>
using namespace std;

namespace {

struct DSU {
    vector<int> parent;
    vector<int> size;

    explicit DSU(const int n) : parent(n), size(n, 1) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(const int x) {
        if (parent[x] == x) {
            return x;
        }
        parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size[a] < size[b]) {
            swap(a, b);
        }
        parent[b] = a;
        size[a] += size[b];
    }
};

struct Point {
    long long x = 0;
    long long y = 0;
};

struct Edge {
    long long dist2 = 0;
    int u = 0;
    int v = 0;

    bool operator<(const Edge& other) const {
        return dist2 < other.dist2;
    }
};

long long squared_distance(const Point& a, const Point& b) {
    const long long dx = a.x - b.x;
    const long long dy = a.y - b.y;
    return dx * dx + dy * dy;
}

bool is_feasible(const int team_size,
                 const int total_points,
                 const vector<Edge>& edges,
                 const long long threshold) {
    DSU dsu(total_points);
    for (const Edge& edge : edges) {
        if (edge.dist2 >= threshold) {
            break;
        }
        dsu.unite(edge.u, edge.v);
    }

    vector<int> component_sizes;
    vector<int> seen(total_points, -1);
    for (int i = 0; i < total_points; ++i) {
        const int root = dsu.find(i);
        if (seen[root] != -1) {
            continue;
        }
        seen[root] = static_cast<int>(component_sizes.size());
        component_sizes.push_back(dsu.size[root]);
    }

    bitset<501> dp;
    dp[0] = 1;
    for (const int size : component_sizes) {
        dp |= (dp << size);
    }
    return dp[team_size];
}

vector<int> build_solution(const int team_size,
                           const int total_points,
                           const vector<Edge>& edges,
                           const long long threshold) {
    DSU dsu(total_points);
    for (const Edge& edge : edges) {
        if (edge.dist2 >= threshold) {
            break;
        }
        dsu.unite(edge.u, edge.v);
    }

    vector<int> component_index(total_points, -1);
    vector<int> component_sizes;
    vector<vector<int>> component_members;
    for (int i = 0; i < total_points; ++i) {
        const int root = dsu.find(i);
        if (component_index[root] == -1) {
            component_index[root] = static_cast<int>(component_sizes.size());
            component_sizes.push_back(0);
            component_members.push_back({});
        }
        const int id = component_index[root];
        ++component_sizes[id];
        component_members[id].push_back(i + 1);
    }

    const int component_count = static_cast<int>(component_sizes.size());
    vector<vector<char>> can(component_count + 1,
                             vector<char>(team_size + 1, 0));
    vector<vector<char>> parent_take(component_count + 1,
                                     vector<char>(team_size + 1, -1));
    can[0][0] = 1;

    for (int i = 0; i < component_count; ++i) {
        for (int sum = 0; sum <= team_size; ++sum) {
            if (!can[i][sum]) {
                continue;
            }
            if (!can[i + 1][sum]) {
                can[i + 1][sum] = 1;
                parent_take[i + 1][sum] = 0;
            }
            const int next_sum = sum + component_sizes[i];
            if (next_sum <= team_size && !can[i + 1][next_sum]) {
                can[i + 1][next_sum] = 1;
                parent_take[i + 1][next_sum] = 1;
            }
        }
    }

    vector<int> blue_team;
    int sum = team_size;
    for (int i = component_count; i >= 1; --i) {
        if (parent_take[i][sum] == 1) {
            for (const int member : component_members[i - 1]) {
                blue_team.push_back(member);
            }
            sum -= component_sizes[i - 1];
        }
    }
    sort(blue_team.begin(), blue_team.end());
    return blue_team;
}

void solve() {
    int team_size;
    cin >> team_size;
    const int total_points = 2 * team_size;

    vector<Point> points(total_points);
    for (Point& point : points) {
        cin >> point.x >> point.y;
    }

    vector<Edge> edges;
    edges.reserve(total_points * (total_points - 1) / 2);
    for (int i = 0; i < total_points; ++i) {
        for (int j = i + 1; j < total_points; ++j) {
            edges.push_back({squared_distance(points[i], points[j]), i, j});
        }
    }
    sort(edges.begin(), edges.end());

    vector<long long> distances;
    distances.reserve(edges.size());
    for (const Edge& edge : edges) {
        if (distances.empty() || distances.back() != edge.dist2) {
            distances.push_back(edge.dist2);
        }
    }

    int low = 0;
    int high = static_cast<int>(distances.size()) - 1;
    int best_index = 0;
    while (low <= high) {
        const int mid = low + (high - low) / 2;
        if (is_feasible(team_size, total_points, edges, distances[mid])) {
            best_index = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    const long long best_dist2 = distances[best_index];
    const vector<int> blue_team =
        build_solution(team_size, total_points, edges, best_dist2);

    cout << fixed << setprecision(6)
         << sqrt(static_cast<long double>(best_dist2)) << '\n';
    for (const int member : blue_team) {
        cout << member << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
