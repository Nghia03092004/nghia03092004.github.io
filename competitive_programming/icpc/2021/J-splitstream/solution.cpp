#include <bits/stdc++.h>
using namespace std;

namespace {

struct Node {
    char type;
    int a;
    int b;
    int c;
};

struct WireInfo {
    int creator;
    int role;
};

long long m;
int n;
int q;
vector<Node> nodes;
vector<WireInfo> wire_info;
vector<long long> length_cache;

long long get_length(int wire) {
    if (wire == 1) {
        return m;
    }
    long long& memo = length_cache[wire];
    if (memo != -1) {
        return memo;
    }

    const WireInfo& info = wire_info[wire];
    const Node& node = nodes[info.creator];

    if (node.type == 'S') {
        long long input_length = get_length(node.a);
        if (info.role == 1) {
            memo = (input_length + 1) / 2;
        } else {
            memo = input_length / 2;
        }
    } else {
        memo = get_length(node.a) + get_length(node.b);
    }

    return memo;
}

long long query_value(int wire, long long k) {
    while (wire != 1) {
        const WireInfo& info = wire_info[wire];
        const Node& node = nodes[info.creator];

        if (node.type == 'S') {
            if (info.role == 1) {
                k = 2 * k - 1;
            } else {
                k = 2 * k;
            }
            wire = node.a;
            continue;
        }

        long long left_length = get_length(node.a);
        long long right_length = get_length(node.b);
        long long interleaved = 2 * min(left_length, right_length);

        if (k <= interleaved) {
            if (k % 2 == 1) {
                k = (k + 1) / 2;
                wire = node.a;
            } else {
                k /= 2;
                wire = node.b;
            }
        } else if (left_length > right_length) {
            k -= right_length;
            wire = node.a;
        } else {
            k -= left_length;
            wire = node.b;
        }
    }

    return k;
}

void solve() {
    cin >> m >> n >> q;

    nodes.resize(n);
    int max_wire = 1;
    for (int i = 0; i < n; ++i) {
        cin >> nodes[i].type >> nodes[i].a >> nodes[i].b >> nodes[i].c;
        max_wire = max(max_wire, max(nodes[i].a, max(nodes[i].b, nodes[i].c)));
    }

    vector<pair<int, long long>> queries(q);
    for (int i = 0; i < q; ++i) {
        cin >> queries[i].first >> queries[i].second;
        max_wire = max(max_wire, queries[i].first);
    }

    wire_info.assign(max_wire + 1, {-1, 0});
    for (int i = 0; i < n; ++i) {
        const Node& node = nodes[i];
        if (node.type == 'S') {
            wire_info[node.b] = {i, 1};
            wire_info[node.c] = {i, 2};
        } else {
            wire_info[node.c] = {i, 0};
        }
    }

    length_cache.assign(max_wire + 1, -1);
    length_cache[1] = m;

    for (const auto& query : queries) {
        int wire = query.first;
        long long k = query.second;
        if (k < 1 || k > get_length(wire)) {
            cout << "none\n";
        } else {
            cout << query_value(wire, k) << '\n';
        }
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
