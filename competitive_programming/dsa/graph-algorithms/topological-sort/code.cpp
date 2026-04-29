vector<int> topological_sort(const vector<vector<int>>& graph) {
    int n = (int)graph.size();
    vector<int> indeg(n, 0);
    for (int v = 0; v < n; ++v) {
        for (int to : graph[v]) {
            ++indeg[to];
        }
    }

    queue<int> q;
    for (int v = 0; v < n; ++v) {
        if (indeg[v] == 0) {
            q.push(v);
        }
    }

    vector<int> order;
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        order.push_back(v);
        for (int to : graph[v]) {
            if (--indeg[to] == 0) {
                q.push(to);
            }
        }
    }

    if ((int)order.size() != n) {
        return {};
    }
    return order;
}
