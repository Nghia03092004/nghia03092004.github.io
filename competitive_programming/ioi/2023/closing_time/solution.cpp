#include <bits/stdc++.h>
using namespace std;

struct Fenwick {
    vector<long long> tree;
    int sz;

    explicit Fenwick(int sz = 0) : tree(sz + 5, 0), sz(sz) {}

    void add(int idx, long long val) {
        for (int i = idx; i <= sz; i += i & -i) {
            tree[i] += val;
        }
    }

    long long query(int idx) const {
        long long ret = 0;
        for (int i = idx; i > 0; i -= i & -i) {
            ret += tree[i];
        }
        return ret;
    }

    int get_highest_with_sum_at_most(long long k) const {
        int idx = 0;
        long long total = 0;
        int lg = 0;
        while ((1 << lg) <= sz) {
            ++lg;
        }
        for (int i = lg - 1; i >= 0; --i) {
            if (idx + (1 << i) <= sz && total + tree[idx + (1 << i)] <= k) {
                idx += 1 << i;
                total += tree[idx];
            }
        }
        return idx;
    }
};

namespace {

vector<vector<pair<int, int>>> adj_list;
vector<long long> p_val, q_val;
vector<bool> in_path_xy;
int N;
long long K;

void dfs_distance(int now, int prv, long long dist,
                  vector<long long>& dist_vec, vector<int>& parent) {
    dist_vec[now] = dist;
    parent[now] = prv;
    for (auto [nxt, w] : adj_list[now]) {
        if (nxt != prv) {
            dfs_distance(nxt, now, dist + w, dist_vec, parent);
        }
    }
}

int calculate_with_common() {
    vector<tuple<long long, long long, int>> sorted_diffs;
    for (int i = 0; i < N; ++i) {
        sorted_diffs.emplace_back(q_val[i] - p_val[i], p_val[i], i);
    }
    sort(sorted_diffs.begin(), sorted_diffs.end());

    vector<pair<long long, int>> sorted_vals;
    for (int i = 0; i < N; ++i) {
        int node = get<2>(sorted_diffs[i]);
        sorted_vals.push_back({2 * p_val[node], node});
        sorted_vals.push_back({q_val[node], -2 * node - 2});
        sorted_vals.push_back({q_val[node], -2 * node - 1});
    }
    sorted_vals.push_back({-1, -1});
    sort(sorted_vals.begin(), sorted_vals.end());

    Fenwick dist_tree(3 * N), q_counter(3 * N), p_counter(3 * N);

    auto get_idx = [&](long long v, int id) {
        return static_cast<int>(
            lower_bound(sorted_vals.begin(), sorted_vals.end(), pair<long long, int>(v, id)) -
            sorted_vals.begin());
    };

    auto get_optimum_leftover = [&](long long left) {
        int highest_idx = dist_tree.get_highest_with_sum_at_most(left);
        long long current_total = dist_tree.query(highest_idx);
        int current_score = static_cast<int>(q_counter.query(highest_idx) +
                                             p_counter.query(highest_idx));

        if (q_counter.query(highest_idx) % 2 == 0) {
            return current_score;
        }

        int highest_q_idx = 1 + q_counter.get_highest_with_sum_at_most(q_counter.query(highest_idx) - 1);
        long long highest_q_val = sorted_vals[highest_q_idx].first;

        if (p_counter.query(highest_idx) != p_counter.query(3 * N)) {
            int next_p_idx = 1 + p_counter.get_highest_with_sum_at_most(p_counter.query(highest_idx));
            long long next_p_val = sorted_vals[next_p_idx].first;
            if (current_total - highest_q_val + next_p_val <= left) {
                return current_score;
            }
        }

        if (p_counter.query(highest_idx) > 0) {
            int highest_p_idx = 1 + p_counter.get_highest_with_sum_at_most(p_counter.query(highest_idx) - 1);
            long long highest_p_val = sorted_vals[highest_p_idx].first;
            if (current_total - highest_p_val + highest_q_val <= left) {
                return current_score;
            }
        }

        return current_score - 1;
    };

    long long total_in_path = 0;
    int score_in_path = 0;
    int ret = 0;

    for (int i = 0; i < N; ++i) {
        if (in_path_xy[i]) {
            total_in_path += p_val[i];
            ++score_in_path;
        } else {
            int idx = get_idx(2 * p_val[i], i);
            dist_tree.add(idx, 2 * p_val[i]);
            p_counter.add(idx, 1);
        }
    }

    for (int border = 0; border < N; ++border) {
        int node = get<2>(sorted_diffs[border]);
        if (in_path_xy[node]) {
            total_in_path += q_val[node] - p_val[node];
            ++score_in_path;
        } else {
            int idx_p = get_idx(2 * p_val[node], node);
            dist_tree.add(idx_p, -2 * p_val[node]);
            p_counter.add(idx_p, -1);

            int idx_q = get_idx(q_val[node], -2 * node - 2);
            dist_tree.add(idx_q, q_val[node]);
            q_counter.add(idx_q, 1);
            dist_tree.add(idx_q + 1, q_val[node]);
            q_counter.add(idx_q + 1, 1);
        }

        if (total_in_path <= K) {
            int opt_leftover = get_optimum_leftover(2 * (K - total_in_path));
            ret = max(ret, score_in_path + opt_leftover);
        }
    }

    return ret;
}

int calculate_without_common() {
    vector<long long> sorted_dist;
    for (int i = 0; i < N; ++i) {
        sorted_dist.push_back(p_val[i]);
        sorted_dist.push_back(q_val[i]);
    }
    sort(sorted_dist.begin(), sorted_dist.end());

    int score = 0;
    long long total = 0;
    for (long long d : sorted_dist) {
        if (total + d <= K) {
            ++score;
            total += d;
        }
    }
    return score;
}

}  // namespace

int max_score(int _N, int X, int Y, long long _K,
              vector<int> U, vector<int> V, vector<int> W) {
    N = _N;
    K = _K;

    adj_list.assign(N, {});
    p_val.assign(N, 0);
    q_val.assign(N, 0);
    in_path_xy.assign(N, false);

    for (int i = 0; i < static_cast<int>(U.size()); ++i) {
        adj_list[U[i]].push_back({V[i], W[i]});
        adj_list[V[i]].push_back({U[i], W[i]});
    }

    vector<int> parent_rooted_x(N);
    vector<long long> dist_from_x(N), dist_from_y(N);
    dfs_distance(Y, -1, 0, dist_from_y, parent_rooted_x);
    dfs_distance(X, -1, 0, dist_from_x, parent_rooted_x);

    for (int i = Y; i != -1; i = parent_rooted_x[i]) {
        in_path_xy[i] = true;
    }
    for (int i = 0; i < N; ++i) {
        p_val[i] = min(dist_from_x[i], dist_from_y[i]);
        q_val[i] = max(dist_from_x[i], dist_from_y[i]);
    }

    int ret = calculate_with_common();
    ret = max(ret, calculate_without_common());
    return ret;
}

int main() {
    int N, X, Y;
    long long K;
    scanf("%d %d %d %lld", &N, &X, &Y, &K);
    vector<int> U(N - 1), V(N - 1), W(N - 1);
    for (int i = 0; i < N - 1; ++i) {
        scanf("%d %d %d", &U[i], &V[i], &W[i]);
    }
    printf("%d\n", max_score(N, X, Y, K, U, V, W));
    return 0;
}
