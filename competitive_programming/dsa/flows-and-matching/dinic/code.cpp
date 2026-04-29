struct Dinic {
    struct Edge {
        int to;
        int rev;
        long long cap;
    };

    int n;
    vector<vector<Edge>> graph;
    vector<int> level;
    vector<int> ptr;

    explicit Dinic(int n) : n(n), graph(n), level(n), ptr(n) {}

    void add_edge(int u, int v, long long cap) {
        Edge a{v, (int)graph[v].size(), cap};
        Edge b{u, (int)graph[u].size(), 0};
        graph[u].push_back(a);
        graph[v].push_back(b);
    }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q;
        level[s] = 0;
        q.push(s);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (const Edge& e : graph[v]) {
                if (e.cap > 0 && level[e.to] == -1) {
                    level[e.to] = level[v] + 1;
                    q.push(e.to);
                }
            }
        }
        return level[t] != -1;
    }

    long long dfs(int v, int t, long long pushed) {
        if (v == t || pushed == 0) {
            return pushed;
        }
        for (int& cid = ptr[v]; cid < (int)graph[v].size(); ++cid) {
            Edge& e = graph[v][cid];
            if (e.cap == 0 || level[e.to] != level[v] + 1) {
                continue;
            }
            long long flow = dfs(e.to, t, min(pushed, e.cap));
            if (flow == 0) {
                continue;
            }
            e.cap -= flow;
            graph[e.to][e.rev].cap += flow;
            return flow;
        }
        return 0;
    }

    long long max_flow(int s, int t) {
        long long flow = 0;
        while (bfs(s, t)) {
            fill(ptr.begin(), ptr.end(), 0);
            while (long long pushed = dfs(s, t, (long long)4e18)) {
                flow += pushed;
            }
        }
        return flow;
    }
};
