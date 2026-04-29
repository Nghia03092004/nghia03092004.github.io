vector<int> bfs_distances(const vector<vector<int>>& graph, int source) {
    int n = (int)graph.size();
    vector<int> dist(n, -1);
    queue<int> q;
    dist[source] = 0;
    q.push(source);
    while (!q.empty()) {
        int v = q.front();
        q.pop();
        for (int to : graph[v]) {
            if (dist[to] == -1) {
                dist[to] = dist[v] + 1;
                q.push(to);
            }
        }
    }
    return dist;
}

vector<int> dfs_order(const vector<vector<int>>& graph, int source) {
    vector<int> order;
    vector<int> seen(graph.size(), 0);
    vector<int> st = {source};
    while (!st.empty()) {
        int v = st.back();
        st.pop_back();
        if (seen[v]) continue;
        seen[v] = 1;
        order.push_back(v);
        for (int i = (int)graph[v].size() - 1; i >= 0; --i) {
            int to = graph[v][i];
            if (!seen[to]) {
                st.push_back(to);
            }
        }
    }
    return order;
}

vector<vector<int>> connected_components(const vector<vector<int>>& graph) {
    int n = (int)graph.size();
    vector<int> seen(n, 0);
    vector<vector<int>> comps;
    for (int start = 0; start < n; ++start) {
        if (seen[start]) continue;
        vector<int> comp;
        stack<int> st;
        st.push(start);
        seen[start] = 1;
        while (!st.empty()) {
            int v = st.top();
            st.pop();
            comp.push_back(v);
            for (int to : graph[v]) {
                if (!seen[to]) {
                    seen[to] = 1;
                    st.push(to);
                }
            }
        }
        comps.push_back(comp);
    }
    return comps;
}
