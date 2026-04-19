#include <bits/stdc++.h>
using namespace std;

struct Vertex {
    long double x, y, z;
};

struct Triangle {
    array<int, 3> v;
    array<int, 3> e;
};

struct ContourPiece {
    int e1, e2;
    long double slope, intercept;
};

struct RollbackDSU {
    struct Change {
        int child;
        int parent_root;
        int old_size;
        long double old_slope;
        long double old_intercept;
    };

    vector<int> parent, sz;
    vector<long double> slope_sum, intercept_sum;
    vector<Change> history;

    void init(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        slope_sum.assign(n, 0);
        intercept_sum.assign(n, 0);
        history.clear();
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        while (parent[x] != x) {
            x = parent[x];
        }
        return x;
    }

    int snapshot() const {
        return (int)history.size();
    }

    void unite(int a, int b, long double add_slope, long double add_intercept) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (sz[a] < sz[b]) {
            swap(a, b);
        }
        history.push_back({b, a, sz[a], slope_sum[a], intercept_sum[a]});
        parent[b] = a;
        sz[a] += sz[b];
        slope_sum[a] += slope_sum[b] + add_slope;
        intercept_sum[a] += intercept_sum[b] + add_intercept;
    }

    void rollback(int snap) {
        while ((int)history.size() > snap) {
            Change step = history.back();
            history.pop_back();
            parent[step.child] = step.child;
            sz[step.parent_root] = step.old_size;
            slope_sum[step.parent_root] = step.old_slope;
            intercept_sum[step.parent_root] = step.old_intercept;
        }
    }

    long double component_length(int x, long double level) {
        x = find(x);
        return slope_sum[x] * level + intercept_sum[x];
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int t;
    cin >> t;
    cout << fixed << setprecision(9);

    while (t--) {
        int w, h, n, m;
        cin >> w >> h >> n >> m;

        vector<Vertex> vertex(n + 1);
        int sw = -1, se = -1, nw = -1, ne = -1;
        for (int i = 1; i <= n; ++i) {
            int x, y, z;
            cin >> x >> y >> z;
            vertex[i] = {(long double)x, (long double)y, (long double)z};
            if (x == 0 && y == 0) sw = i;
            if (x == w && y == 0) se = i;
            if (x == 0 && y == h) nw = i;
            if (x == w && y == h) ne = i;
        }

        long double low_ok =
            max(min(vertex[sw].z, vertex[nw].z), min(vertex[se].z, vertex[ne].z));
        long double high_ok =
            min(max(vertex[sw].z, vertex[nw].z), max(vertex[se].z, vertex[ne].z));

        auto edge_key = [](int a, int b) -> unsigned long long {
            if (a > b) swap(a, b);
            return (unsigned long long)(unsigned int)a << 32 | (unsigned int)b;
        };

        unordered_map<unsigned long long, int> edge_id;
        edge_id.reserve(3 * m + 10);
        vector<pair<int, int>> edges;
        edges.reserve(3 * m);

        auto get_or_create_edge = [&](int a, int b) {
            unsigned long long key = edge_key(a, b);
            auto it = edge_id.find(key);
            if (it != edge_id.end()) {
                return it->second;
            }
            int id = (int)edges.size();
            edges.push_back({min(a, b), max(a, b)});
            edge_id[key] = id;
            return id;
        };

        vector<Triangle> tri(m);
        vector<vector<int>> incident(n + 1);
        for (int i = 0; i < m; ++i) {
            int a, b, c;
            cin >> a >> b >> c;
            tri[i] = {{{a, b, c}}, {{get_or_create_edge(a, b), get_or_create_edge(b, c),
                                     get_or_create_edge(c, a)}}};
            incident[a].push_back(i);
            incident[b].push_back(i);
            incident[c].push_back(i);
        }

        auto lookup_edge = [&](int a, int b) {
            return edge_id.at(edge_key(a, b));
        };
        int west_edge = lookup_edge(sw, nw);
        int east_edge = lookup_edge(se, ne);

        vector<int> order(n);
        iota(order.begin(), order.end(), 1);
        sort(order.begin(), order.end(), [&](int a, int b) { return vertex[a].z < vertex[b].z; });
        vector<int> rank(n + 1);
        for (int i = 0; i < n; ++i) {
            rank[order[i]] = i;
        }

        int base = 1;
        while (base < n) {
            base <<= 1;
        }
        vector<vector<ContourPiece>> seg(2 * base);

        auto add_range = [&](int l, int r, const ContourPiece& piece) {
            for (l += base, r += base; l < r; l >>= 1, r >>= 1) {
                if (l & 1) seg[l++].push_back(piece);
                if (r & 1) seg[--r].push_back(piece);
            }
        };

        auto contour_norm = [&](int pivot, int a, int b) {
            const Vertex& s = vertex[pivot];
            const Vertex& p = vertex[a];
            const Vertex& q = vertex[b];
            long double dx = (p.x - s.x) / (p.z - s.z) - (q.x - s.x) / (q.z - s.z);
            long double dy = (p.y - s.y) / (p.z - s.z) - (q.y - s.y) / (q.z - s.z);
            return hypotl(dx, dy);
        };

        for (const Triangle& cur : tri) {
            array<int, 3> ord = {0, 1, 2};
            sort(ord.begin(), ord.end(), [&](int lhs, int rhs) {
                return vertex[cur.v[lhs]].z < vertex[cur.v[rhs]].z;
            });

            int lo = ord[0];
            int mid = ord[1];
            int hi = ord[2];

            int low_v = cur.v[lo];
            int mid_v = cur.v[mid];
            int high_v = cur.v[hi];

            int l = rank[low_v] + 1;
            int r = rank[mid_v];
            if (l < r) {
                long double k = contour_norm(low_v, mid_v, high_v);
                add_range(l, r, {cur.e[lo], cur.e[(lo + 2) % 3], k, -vertex[low_v].z * k});
            }

            l = rank[mid_v] + 1;
            r = rank[high_v];
            if (l < r) {
                long double k = contour_norm(high_v, low_v, mid_v);
                add_range(l, r,
                          {cur.e[hi], cur.e[(hi + 2) % 3], -k, vertex[high_v].z * k});
            }
        }

        RollbackDSU dsu;
        dsu.init((int)edges.size());

        const long double INF = 1e100L;
        long double answer = INF;

        function<void(int, int, int)> dfs = [&](int node, int l, int r) {
            int snap = dsu.snapshot();
            for (const ContourPiece& piece : seg[node]) {
                dsu.unite(piece.e1, piece.e2, piece.slope, piece.intercept);
            }

            if (r - l == 1) {
                if (l < n) {
                    int v = order[l];
                    long double level = vertex[v].z;
                    if (low_ok <= level && level <= high_ok) {
                        int west_root = dsu.find(west_edge);
                        int east_root = dsu.find(east_edge);

                        long double best_here = INF;
                        if (west_root == east_root) {
                            best_here = dsu.component_length(west_root, level);
                        }

                        long double best_from_west = INF;
                        long double best_from_east = INF;
                        if (v == sw || v == nw) best_from_west = 0;
                        if (v == se || v == ne) best_from_east = 0;

                        for (int tid : incident[v]) {
                            const Triangle& cur = tri[tid];
                            int pos = 0;
                            while (cur.v[pos] != v) {
                                ++pos;
                            }

                            int a = cur.v[(pos + 1) % 3];
                            int b = cur.v[(pos + 2) % 3];
                            long double da = vertex[a].z - level;
                            long double db = vertex[b].z - level;
                            if (!((da < 0 && db > 0) || (da > 0 && db < 0))) {
                                continue;
                            }

                            int opp_edge = cur.e[(pos + 1) % 3];
                            long double t_on_edge =
                                (level - vertex[a].z) / (vertex[b].z - vertex[a].z);
                            long double px =
                                vertex[a].x + t_on_edge * (vertex[b].x - vertex[a].x);
                            long double py =
                                vertex[a].y + t_on_edge * (vertex[b].y - vertex[a].y);
                            long double extra = hypotl(px - vertex[v].x, py - vertex[v].y);

                            int root = dsu.find(opp_edge);
                            if (root == west_root) {
                                long double base_cost =
                                    (opp_edge == west_edge ? 0 : dsu.component_length(root, level));
                                best_from_west = min(best_from_west, base_cost + extra);
                            }
                            if (root == east_root) {
                                long double base_cost =
                                    (opp_edge == east_edge ? 0 : dsu.component_length(root, level));
                                best_from_east = min(best_from_east, base_cost + extra);
                            }
                        }

                        if (best_from_west < INF / 2 && best_from_east < INF / 2) {
                            best_here = min(best_here, best_from_west + best_from_east);
                        }
                        answer = min(answer, best_here);
                    }
                }
            } else {
                int mid = (l + r) / 2;
                dfs(node * 2, l, mid);
                dfs(node * 2 + 1, mid, r);
            }

            dsu.rollback(snap);
        };

        dfs(1, 0, base);

        if (answer >= INF / 2) {
            cout << "impossible\n";
        } else {
            cout << answer << '\n';
        }
    }
    return 0;
}
