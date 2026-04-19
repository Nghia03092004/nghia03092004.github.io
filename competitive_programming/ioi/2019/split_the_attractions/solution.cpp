#include <bits/stdc++.h>
using namespace std;

static const int MAXN = 200005;
vector<int> adj[MAXN], tree_adj[MAXN];
int sub_sz[MAXN], par[MAXN], ans_label[MAXN];
bool visited[MAXN];
int N, M;

void build_spanning_tree() {
    fill(visited, visited + N, false);
    queue<int> q;
    q.push(0);
    visited[0] = true;
    par[0] = -1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                par[v] = u;
                tree_adj[u].push_back(v);
                tree_adj[v].push_back(u);
                q.push(v);
            }
        }
    }
}

int compute_size(int u, int p) {
    sub_sz[u] = 1;
    for (int v : tree_adj[u])
        if (v != p)
            sub_sz[u] += compute_size(v, u);
    return sub_sz[u];
}

int find_centroid(int u, int p, int tree_size) {
    for (int v : tree_adj[u])
        if (v != p && sub_sz[v] > tree_size / 2)
            return find_centroid(v, u, tree_size);
    return u;
}

// Extract exactly 'need' nodes from subtree rooted at u (parent p)
// Returns number of nodes labeled
int extract(int u, int p, int need, int label) {
    if (need <= 0) return 0;
    ans_label[u] = label;
    int taken = 1;
    for (int v : tree_adj[u]) {
        if (v != p && ans_label[v] == -1) {
            if (taken >= need) break;
            int can_take = min(sub_sz[v], need - taken);
            taken += extract(v, u, can_take, label);
        }
    }
    return taken;
}

vector<int> find_split(int n, int a, int b, int c,
                        vector<int> p, vector<int> q) {
    N = n;
    M = p.size();
    for (int i = 0; i < N; i++) {
        adj[i].clear();
        tree_adj[i].clear();
        ans_label[i] = -1;
    }
    for (int i = 0; i < M; i++) {
        adj[p[i]].push_back(q[i]);
        adj[q[i]].push_back(p[i]);
    }

    // Sort sizes, remember original labels (1, 2, 3)
    int sizes[3] = {a, b, c};
    int labels[3] = {1, 2, 3};
    // Sort so sizes[0] <= sizes[1] <= sizes[2]
    for (int i = 0; i < 3; i++)
        for (int j = i + 1; j < 3; j++)
            if (sizes[i] > sizes[j]) {
                swap(sizes[i], sizes[j]);
                swap(labels[i], labels[j]);
            }

    build_spanning_tree();
    compute_size(0, -1);
    int centroid = find_centroid(0, -1, N);

    // Re-root at centroid
    for (int i = 0; i < N; i++) tree_adj[i].clear();
    fill(visited, visited + N, false);
    // BFS to build rooted tree at centroid
    queue<int> bfs;
    bfs.push(centroid);
    visited[centroid] = true;
    par[centroid] = -1;
    vector<int> order;
    while (!bfs.empty()) {
        int u = bfs.front(); bfs.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                par[v] = u;
                tree_adj[u].push_back(v);
                bfs.push(v);
            }
        }
    }
    // Compute subtree sizes with centroid as root
    for (int i = order.size() - 1; i >= 0; i--) {
        int u = order[i];
        sub_sz[u] = 1;
        for (int v : tree_adj[u])
            sub_sz[u] += sub_sz[v];
    }

    int small = sizes[0]; // smallest group

    // Try to find a connected subtree of exactly 'small' nodes
    // Strategy: find a child subtree of centroid with size >= small,
    // then extract exactly small nodes from it.
    // If no single child subtree >= small, merge children subtrees.

    // Check children of centroid
    int big_child = -1;
    for (int v : tree_adj[centroid]) {
        if (sub_sz[v] >= small) {
            big_child = v;
            break;
        }
    }

    if (big_child != -1) {
        // Extract 'small' from subtree of big_child
        extract(big_child, centroid, small, labels[0]);
    } else {
        // Merge subtrees greedily
        int taken = 0;
        for (int v : tree_adj[centroid]) {
            if (taken >= small) break;
            int need = small - taken;
            if (sub_sz[v] <= need) {
                taken += extract(v, centroid, sub_sz[v], labels[0]);
            } else {
                taken += extract(v, centroid, need, labels[0]);
            }
        }
        if (taken < small) {
            ans_label[centroid] = labels[0];
        }
    }

    // Now split remaining into sizes[1] and sizes[2]
    // The remaining nodes form a connected subtree (centroid + unlabeled subtrees)
    // Extract sizes[1] from the remaining, rest gets sizes[2]

    // Find connected component of unlabeled nodes containing centroid
    // and extract sizes[1] from it
    if (ans_label[centroid] == -1) {
        // Centroid is in the remaining part
        extract(centroid, -1, sizes[1], labels[1]);
    } else {
        // Centroid was taken by first group
        // Find a remaining node adjacent to centroid
        // Actually, let's find the component differently
        // The remaining nodes with centroid removed might be disconnected
        // We need to handle this carefully.

        // Re-approach: assign from any remaining node
        for (int v : tree_adj[centroid]) {
            if (ans_label[v] == -1) {
                extract(v, centroid, sizes[1], labels[1]);
                break;
            }
        }
    }

    // Label all remaining as labels[2]
    for (int i = 0; i < N; i++)
        if (ans_label[i] == -1)
            ans_label[i] = labels[2];

    return vector<int>(ans_label, ans_label + N);
}

int main() {
    int n, m, a, b, c;
    scanf("%d %d %d %d %d", &n, &m, &a, &b, &c);
    vector<int> p(m), q(m);
    for (int i = 0; i < m; i++)
        scanf("%d %d", &p[i], &q[i]);
    vector<int> res = find_split(n, a, b, c, p, q);
    for (int i = 0; i < n; i++)
        printf("%d%c", res[i], " \n"[i == n - 1]);
    return 0;
}
