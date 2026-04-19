#include <bits/stdc++.h>
using namespace std;

using ll = long long;

struct MaxTree {
    int n;
    vector<int> mx;

    explicit MaxTree(const vector<int> &h) {
        n = (int)h.size();
        mx.assign(4 * n, 0);
        build(1, 0, n - 1, h);
    }

    void build(int node, int l, int r, const vector<int> &h) {
        if (l == r) {
            mx[node] = h[l];
            return;
        }
        int mid = (l + r) / 2;
        build(node * 2, l, mid, h);
        build(node * 2 + 1, mid + 1, r, h);
        mx[node] = max(mx[node * 2], mx[node * 2 + 1]);
    }

    int find_rightmost(int node, int l, int r, int ql, int qr, int need) const {
        if (qr < l || r < ql || mx[node] < need) return -1;
        if (l == r) return l;
        int mid = (l + r) / 2;
        int right = find_rightmost(node * 2 + 1, mid + 1, r, ql, qr, need);
        if (right != -1) return right;
        return find_rightmost(node * 2, l, mid, ql, qr, need);
    }

    int find_leftmost(int node, int l, int r, int ql, int qr, int need) const {
        if (qr < l || r < ql || mx[node] < need) return -1;
        if (l == r) return l;
        int mid = (l + r) / 2;
        int left = find_leftmost(node * 2, l, mid, ql, qr, need);
        if (left != -1) return left;
        return find_leftmost(node * 2 + 1, mid + 1, r, ql, qr, need);
    }
};

struct AssignTree {
    int n;
    vector<int> lazy;

    explicit AssignTree(int n_) : n(n_), lazy(4 * n_, -1) {}

    void assign_range(int node, int l, int r, int ql, int qr, int val) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            lazy[node] = val;
            return;
        }
        int mid = (l + r) / 2;
        if (lazy[node] != -1) {
            lazy[node * 2] = lazy[node];
            lazy[node * 2 + 1] = lazy[node];
            lazy[node] = -1;
        }
        assign_range(node * 2, l, mid, ql, qr, val);
        assign_range(node * 2 + 1, mid + 1, r, ql, qr, val);
    }

    int point_query(int node, int l, int r, int pos) const {
        if (l == r || lazy[node] != -1) return max(lazy[node], 0);
        int mid = (l + r) / 2;
        if (pos <= mid) return point_query(node * 2, l, mid, pos);
        return point_query(node * 2 + 1, mid + 1, r, pos);
    }
};

struct Skywalk {
    int l, r, y;
};

struct VerticalEdge {
    int building;
    int low;
    int high;
};

long long min_distance(vector<int> x, vector<int> h,
                       vector<int> l, vector<int> r, vector<int> y,
                       int s, int g) {
    if (s == g) return 0;

    int n = (int)x.size();
    vector<Skywalk> walks;
    walks.reserve(l.size() * 5);
    for (int i = 0; i < (int)l.size(); ++i) walks.push_back({l[i], r[i], y[i]});

    MaxTree tall(h);
    auto split_at = [&](int pivot) {
        vector<Skywalk> next;
        next.reserve(walks.size() * 3);
        for (const Skywalk &sw : walks) {
            if (!(sw.l < pivot && pivot < sw.r)) {
                next.push_back(sw);
                continue;
            }
            int left = tall.find_rightmost(1, 0, n - 1, sw.l, pivot, sw.y);
            int right = tall.find_leftmost(1, 0, n - 1, pivot, sw.r, sw.y);
            if (sw.l < left) next.push_back({sw.l, left, sw.y});
            if (left < right) next.push_back({left, right, sw.y});
            if (right < sw.r) next.push_back({right, sw.r, sw.y});
        }
        walks.swap(next);
    };

    split_at(s);
    if (g != s) split_at(g);

    vector<int> order(walks.size());
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (walks[a].y != walks[b].y) return walks[a].y < walks[b].y;
        if (walks[a].l != walks[b].l) return walks[a].l < walks[b].l;
        return walks[a].r < walks[b].r;
    });

    vector<vector<int>> sky_nodes(walks.size());
    vector<pair<int, int>> points;
    points.reserve(walks.size() * 6 + 4);
    vector<VerticalEdge> verticals;
    verticals.reserve(walks.size() * 4);

    auto add_point = [&](int building, int ht) {
        points.push_back({building, ht});
    };

    add_point(s, 0);
    add_point(g, 0);

    AssignTree cover(n);
    for (int i = 0; i < (int)order.size();) {
        int j = i;
        while (j < (int)order.size() && walks[order[i]].y == walks[order[j]].y) ++j;

        for (int k = i; k < j; ++k) {
            int id = order[k];
            const Skywalk &sw = walks[id];
            add_point(sw.l, sw.y);
            add_point(sw.r, sw.y);

            int pred_l = cover.point_query(1, 0, n - 1, sw.l);
            if (pred_l > 0 || sw.l == s || sw.l == g) {
                int low = pred_l > 0 ? pred_l : 0;
                add_point(sw.l, low);
                verticals.push_back({sw.l, low, sw.y});
            }

            int pred_r = cover.point_query(1, 0, n - 1, sw.r);
            if (pred_r > 0 || sw.r == s || sw.r == g) {
                int low = pred_r > 0 ? pred_r : 0;
                add_point(sw.r, low);
                verticals.push_back({sw.r, low, sw.y});
            }
        }

        for (int k = i; k < j; ++k) {
            int id = order[k];
            cover.assign_range(1, 0, n - 1, walks[id].l, walks[id].r, walks[id].y);
        }
        i = j;
    }

    sort(points.begin(), points.end());
    points.erase(unique(points.begin(), points.end()), points.end());

    unordered_map<long long, int> node_id;
    node_id.reserve(points.size() * 2 + 8);
    for (int i = 0; i < (int)points.size(); ++i) {
        long long key = (static_cast<long long>(points[i].first) << 32) ^
                        static_cast<unsigned int>(points[i].second);
        node_id[key] = i;
    }

    vector<vector<pair<int, ll>>> adj(points.size());
    auto add_edge = [&](int u, int v, ll wgt) {
        adj[u].push_back({v, wgt});
        adj[v].push_back({u, wgt});
    };

    for (const VerticalEdge &ve : verticals) {
        long long key_low = (static_cast<long long>(ve.building) << 32) ^
                            static_cast<unsigned int>(ve.low);
        long long key_high = (static_cast<long long>(ve.building) << 32) ^
                             static_cast<unsigned int>(ve.high);
        add_edge(node_id[key_low], node_id[key_high], ve.high - ve.low);
    }

    unordered_map<int, vector<pair<pair<int, int>, int>>> by_height;
    by_height.reserve(walks.size() * 2 + 1);
    for (int i = 0; i < (int)walks.size(); ++i) {
        by_height[walks[i].y].push_back({{walks[i].l, walks[i].r}, i});
    }
    for (auto &entry : by_height) {
        auto &vec = entry.second;
        sort(vec.begin(), vec.end());
    }

    for (const auto &[building, ht] : points) {
        if (ht == 0) continue;
        auto it = by_height.find(ht);
        if (it == by_height.end()) continue;
        const auto &vec = it->second;
        int pos = lower_bound(
            vec.begin(), vec.end(),
            make_pair(make_pair(building + 1, numeric_limits<int>::min()), numeric_limits<int>::min())) -
                  vec.begin();

        auto attach = [&](int idx) {
            if (idx < 0 || idx >= (int)vec.size()) return;
            int id = vec[idx].second;
            const Skywalk &sw = walks[id];
            if (sw.l <= building && building <= sw.r) sky_nodes[id].push_back(building);
        };

        attach(pos - 1);
        attach(pos - 2);
    }

    for (int i = 0; i < (int)walks.size(); ++i) {
        auto &vec = sky_nodes[i];
        sort(vec.begin(), vec.end());
        vec.erase(unique(vec.begin(), vec.end()), vec.end());
        for (int j = 0; j + 1 < (int)vec.size(); ++j) {
            int a = vec[j];
            int b = vec[j + 1];
            long long key_a = (static_cast<long long>(a) << 32) ^
                              static_cast<unsigned int>(walks[i].y);
            long long key_b = (static_cast<long long>(b) << 32) ^
                              static_cast<unsigned int>(walks[i].y);
            add_edge(node_id[key_a], node_id[key_b], (ll)x[b] - x[a]);
        }
    }

    long long src_key = (static_cast<long long>(s) << 32);
    long long dst_key = (static_cast<long long>(g) << 32);
    int src = node_id[src_key];
    int dst = node_id[dst_key];

    const ll INF = (1LL << 62);
    vector<ll> dist(points.size(), INF);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>> pq;
    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d != dist[u]) continue;
        if (u == dst) return d;
        for (auto [v, wgt] : adj[u]) {
            if (d + wgt < dist[v]) {
                dist[v] = d + wgt;
                pq.push({dist[v], v});
            }
        }
    }

    return -1;
}

int main() {
    int n, m;
    scanf("%d %d", &n, &m);
    vector<int> x(n), h(n);
    for (int i = 0; i < n; ++i) scanf("%d %d", &x[i], &h[i]);
    vector<int> l(m), r(m), y(m);
    for (int i = 0; i < m; ++i) scanf("%d %d %d", &l[i], &r[i], &y[i]);
    int s, g;
    scanf("%d %d", &s, &g);
    printf("%lld\n", min_distance(x, h, l, r, y, s, g));
    return 0;
}
