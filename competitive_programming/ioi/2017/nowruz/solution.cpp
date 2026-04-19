#include <bits/stdc++.h>
using namespace std;

namespace {

const int DR[4] = {-1, 0, 1, 0};
const int DC[4] = {0, 1, 0, -1};

struct Attempt {
    vector<char> used;
    int leaves = 0;
    int cells = 0;
};

int H, W, target_leaves;
vector<string> garden;
vector<int> free_cells;

inline int cell_id(int r, int c) { return r * W + c; }
inline int row_of(int id) { return id / W; }
inline int col_of(int id) { return id % W; }

bool is_free_cell(int r, int c) {
    return 0 <= r && r < H && 0 <= c && c < W && garden[r][c] == '.';
}

vector<int> neighbors(int id) {
    vector<int> out;
    int r = row_of(id), c = col_of(id);
    for (int dir = 0; dir < 4; ++dir) {
        int nr = r + DR[dir];
        int nc = c + DC[dir];
        if (is_free_cell(nr, nc)) out.push_back(cell_id(nr, nc));
    }
    return out;
}

struct Builder {
    vector<char> used;
    vector<unsigned char> degree;
    vector<unsigned char> adj_tree;
    int leaves = 0;
    int cells = 0;

    Builder() : used(H * W, 0), degree(H * W, 0), adj_tree(H * W, 0) {}

    void adjust_degree(int id, int delta) {
        if (degree[id] == 1) --leaves;
        degree[id] = static_cast<unsigned char>(degree[id] + delta);
        if (degree[id] == 1) ++leaves;
    }

    void add_cell(int id) {
        if (used[id]) return;
        used[id] = 1;
        ++cells;
        degree[id] = 0;

        for (int to : neighbors(id)) {
            if (used[to]) {
                adjust_degree(id, 1);
                adjust_degree(to, 1);
            } else {
                ++adj_tree[to];
            }
        }
    }

    vector<int> addable_leaves(int center) const {
        vector<int> extra;
        for (int to : neighbors(center)) {
            if (used[to]) continue;
            if (adj_tree[to] == 0) extra.push_back(to);
        }
        return extra;
    }

    int unique_parent(int id) const {
        for (int to : neighbors(id)) {
            if (used[to]) return to;
        }
        return -1;
    }

    Attempt finish() const { return {used, leaves, cells}; }
};

bool better_attempt(const Attempt &a, const Attempt &b) {
    if (a.leaves != b.leaves) return a.leaves > b.leaves;
    return a.cells > b.cells;
}

Attempt build_scan(int start, mt19937 &rng) {
    Builder cur;
    cur.add_cell(start);

    vector<int> order = free_cells;
    bool changed = true;
    while (changed) {
        changed = false;
        shuffle(order.begin(), order.end(), rng);
        for (int id : order) {
            if (cur.used[id] || cur.adj_tree[id] != 1) continue;
            cur.add_cell(id);
            changed = true;
        }
    }
    return cur.finish();
}

Attempt build_greedy(int start, mt19937 &rng) {
    Builder cur;
    cur.add_cell(start);

    vector<int> initial = neighbors(start);
    shuffle(initial.begin(), initial.end(), rng);
    for (int id : initial) {
        if (!cur.used[id] && cur.adj_tree[id] == 1) cur.add_cell(id);
    }

    while (true) {
        int best_immediate = -1;
        int best_net = INT_MIN;
        int best_seed_degree = -1;
        vector<pair<int, vector<int>>> options;

        for (int center : free_cells) {
            if (cur.used[center] || cur.adj_tree[center] != 1) continue;

            vector<int> extra = cur.addable_leaves(center);
            int immediate = max(1, (int)extra.size());
            int parent = cur.unique_parent(center);
            int net = immediate - (parent != -1 && cur.degree[parent] == 1 ? 1 : 0);
            int seed_degree = (int)neighbors(center).size();

            if (immediate > best_immediate ||
                (immediate == best_immediate && net > best_net) ||
                (immediate == best_immediate && net == best_net && seed_degree > best_seed_degree)) {
                best_immediate = immediate;
                best_net = net;
                best_seed_degree = seed_degree;
                options.clear();
            }
            if (immediate == best_immediate && net == best_net && seed_degree == best_seed_degree) {
                options.push_back({center, std::move(extra)});
            }
        }

        if (options.empty()) break;
        auto chosen = options[uniform_int_distribution<int>(0, (int)options.size() - 1)(rng)];
        cur.add_cell(chosen.first);
        shuffle(chosen.second.begin(), chosen.second.end(), rng);
        for (int leaf : chosen.second) {
            if (!cur.used[leaf] && cur.adj_tree[leaf] == 1) cur.add_cell(leaf);
        }
    }

    return cur.finish();
}

bool is_valid_tree(const vector<char> &used) {
    int start = -1;
    int nodes = 0;
    int twice_edges = 0;
    for (int id : free_cells) {
        if (!used[id]) continue;
        if (start == -1) start = id;
        ++nodes;
        for (int to : neighbors(id)) {
            if (used[to]) ++twice_edges;
        }
    }

    if (nodes <= 1) return true;
    if (twice_edges / 2 != nodes - 1) return false;

    vector<char> vis(H * W, 0);
    queue<int> q;
    q.push(start);
    vis[start] = 1;
    int seen = 0;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        ++seen;
        for (int to : neighbors(v)) {
            if (!used[to] || vis[to]) continue;
            vis[to] = 1;
            q.push(to);
        }
    }
    return seen == nodes;
}

uint64_t input_hash() {
    uint64_t h = 1469598103934665603ULL;
    auto mix = [&](uint64_t x) {
        h ^= x;
        h *= 1099511628211ULL;
    };
    mix(H);
    mix(W);
    mix(target_leaves);
    for (const string &row : garden) {
        for (unsigned char ch : row) mix(ch);
    }
    return h;
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> H >> W >> target_leaves;
    garden.resize(H);
    for (int r = 0; r < H; ++r) cin >> garden[r];

    for (int r = 0; r < H; ++r) {
        for (int c = 0; c < W; ++c) {
            if (garden[r][c] == '.') free_cells.push_back(cell_id(r, c));
        }
    }

    if (free_cells.empty()) {
        for (const string &row : garden) cout << row << '\n';
        return 0;
    }

    uint64_t seed = input_hash();
    mt19937 rng((uint32_t)(seed ^ (seed >> 32)));

    vector<int> seed_order = free_cells;
    stable_sort(seed_order.begin(), seed_order.end(), [&](int a, int b) {
        int da = (int)neighbors(a).size();
        int db = (int)neighbors(b).size();
        if (da != db) return da > db;
        return a < b;
    });

    vector<int> starts;
    vector<char> taken(H * W, 0);
    int deterministic = min<int>((int)seed_order.size(), 24);
    for (int i = 0; i < deterministic; ++i) {
        starts.push_back(seed_order[i]);
        taken[seed_order[i]] = 1;
    }

    vector<int> shuffled = free_cells;
    shuffle(shuffled.begin(), shuffled.end(), rng);
    int extra_runs = min<int>((int)free_cells.size(), 96);
    for (int id : shuffled) {
        if ((int)starts.size() >= deterministic + extra_runs) break;
        if (taken[id]) continue;
        starts.push_back(id);
        taken[id] = 1;
    }

    Attempt best;
    best.used.assign(H * W, 0);
    best.leaves = -1;

    for (int start : starts) {
        Attempt cand1 = build_greedy(start, rng);
        if (is_valid_tree(cand1.used) && better_attempt(cand1, best)) best = cand1;
        if (best.leaves >= target_leaves) break;

        Attempt cand2 = build_scan(start, rng);
        if (is_valid_tree(cand2.used) && better_attempt(cand2, best)) best = cand2;
        if (best.leaves >= target_leaves) break;
    }

    if (best.leaves < 0) {
        best.used.assign(H * W, 0);
        best.used[free_cells[0]] = 1;
    }

    for (int r = 0; r < H; ++r) {
        string out = garden[r];
        for (int c = 0; c < W; ++c) {
            if (garden[r][c] != '.') continue;
            out[c] = best.used[cell_id(r, c)] ? '.' : 'X';
        }
        cout << out << '\n';
    }
    return 0;
}
