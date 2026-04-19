#include <bits/stdc++.h>
using namespace std;

namespace {

struct Result {
    int start = 0;
    vector<long long> dp;
    long long best = 0;
};

struct Frame {
    int v = 0;
    int next_child = 0;
    long long sign_cost = 0;
    long long sum_child_speeds = 0;
    int child_count = 0;
    bool has_sum = false;
    int no_sign_start = 0;
    vector<long long> sum_dp;
};

int n;
long long c;
vector<vector<pair<int, int>>> graph;
vector<vector<int>> children;
vector<int> parent_speed;
vector<int> parent_speed_index;
vector<int> degree_list;
vector<int> subtree_size;
vector<int> all_speeds;

void merge_child(Frame& frame, int child, Result child_result) {
    frame.sign_cost += child_result.best;
    frame.sum_child_speeds += parent_speed[child];

    if (!frame.has_sum) {
        frame.no_sign_start = child_result.start;
        frame.sum_dp = move(child_result.dp);
        frame.has_sum = true;
        return;
    }

    int new_start = max(frame.no_sign_start, child_result.start);
    int new_len = int(all_speeds.size()) - new_start;
    vector<long long> merged(new_len);

    int offset_sum = new_start - frame.no_sign_start;
    int offset_child = new_start - child_result.start;
    for (int i = 0; i < new_len; ++i) {
        merged[i] = frame.sum_dp[i + offset_sum] + child_result.dp[i + offset_child];
    }

    frame.sum_dp = move(merged);
    frame.no_sign_start = new_start;
}

Result finalize_node(const Frame& frame, long long& answer) {
    if (frame.v == 0 && frame.child_count == 0) {
        answer = 0;
        return {};
    }

    if (frame.child_count == 0) {
        int start = parent_speed_index[frame.v];
        return {start, vector<long long>(all_speeds.size() - start, 0), 0};
    }

    if (frame.v == 0) {
        answer = frame.sign_cost;
        for (int i = 0; i < int(frame.sum_dp.size()); ++i) {
            long long speed = all_speeds[frame.no_sign_start + i];
            long long no_sign_cost =
                frame.sum_dp[i] + 1LL * frame.child_count * speed - frame.sum_child_speeds;
            answer = min(answer, no_sign_cost);
        }
        return {};
    }

    int start = parent_speed_index[frame.v];
    vector<long long> dp(all_speeds.size() - start, frame.sign_cost);

    int begin = max(start, frame.no_sign_start);
    for (int idx = begin; idx < int(all_speeds.size()); ++idx) {
        long long speed = all_speeds[idx];
        long long no_sign_cost =
            frame.sum_dp[idx - frame.no_sign_start] + 1LL * frame.child_count * speed - frame.sum_child_speeds;
        dp[idx - start] = min(frame.sign_cost, no_sign_cost);
    }

    long long best = (1LL << 62);
    for (int idx = start; idx < int(all_speeds.size()); ++idx) {
        long long branch_cost = dp[idx - start] + (all_speeds[idx] - parent_speed[frame.v]);
        best = min(best, branch_cost);
    }

    return {start, move(dp), best};
}

long long run_dp() {
    long long answer = 0;
    vector<Frame> stack;
    stack.reserve(n);

    Frame root;
    root.v = 0;
    root.sign_cost = (degree_list[0] >= 2 ? 1LL * degree_list[0] * c : 0LL);
    root.child_count = int(children[0].size());
    stack.push_back(move(root));

    while (!stack.empty()) {
        Frame& current = stack.back();

        if (current.next_child < int(children[current.v].size())) {
            int child = children[current.v][current.next_child++];

            Frame child_frame;
            child_frame.v = child;
            child_frame.sign_cost = (degree_list[child] >= 2 ? 1LL * degree_list[child] * c : 0LL);
            child_frame.child_count = int(children[child].size());
            stack.push_back(move(child_frame));
            continue;
        }

        Result result = finalize_node(current, answer);
        int finished = current.v;
        stack.pop_back();

        if (stack.empty()) {
            break;
        }

        merge_child(stack.back(), finished, move(result));
    }

    return answer;
}

void build_rooted_tree() {
    vector<int> parent(n, -1);
    vector<int> order;
    order.reserve(n);
    order.push_back(0);
    parent[0] = 0;

    for (int i = 0; i < int(order.size()); ++i) {
        int v = order[i];
        for (const auto& edge : graph[v]) {
            int to = edge.first;
            int speed = edge.second;
            if (parent[to] != -1) {
                continue;
            }
            parent[to] = v;
            parent_speed[to] = speed;
            children[v].push_back(to);
            order.push_back(to);
        }
    }

    subtree_size.assign(n, 1);
    for (int i = n - 1; i >= 1; --i) {
        int v = order[i];
        subtree_size[parent[v]] += subtree_size[v];
    }

    for (int v = 0; v < n; ++v) {
        sort(children[v].begin(), children[v].end(), [&](int lhs, int rhs) {
            return subtree_size[lhs] > subtree_size[rhs];
        });
    }
}

void solve() {
    cin >> n >> c;

    if (n == 1) {
        cout << 0 << '\n';
        return;
    }

    graph.assign(n, {});
    children.assign(n, {});
    parent_speed.assign(n, 0);
    parent_speed_index.assign(n, 0);
    degree_list.assign(n, 0);
    all_speeds.clear();
    all_speeds.reserve(n - 1);

    for (int i = 0; i < n - 1; ++i) {
        int u, v, s;
        cin >> u >> v >> s;
        --u;
        --v;

        graph[u].push_back({v, s});
        graph[v].push_back({u, s});
        ++degree_list[u];
        ++degree_list[v];
        all_speeds.push_back(s);
    }

    sort(all_speeds.begin(), all_speeds.end());
    all_speeds.erase(unique(all_speeds.begin(), all_speeds.end()), all_speeds.end());

    for (int v = 1; v < n; ++v) {
        parent_speed_index[v] = 0;
    }

    build_rooted_tree();
    for (int v = 1; v < n; ++v) {
        parent_speed_index[v] =
            int(lower_bound(all_speeds.begin(), all_speeds.end(), parent_speed[v]) - all_speeds.begin());
    }

    cout << run_dp() << '\n';
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
