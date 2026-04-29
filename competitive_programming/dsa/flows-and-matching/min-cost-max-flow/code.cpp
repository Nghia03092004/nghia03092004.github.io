struct MinCostMaxFlow {
    struct Edge {
        int to;
        int rev;
        long long cap;
        long long cost;
    };

    int n;
    vector<vector<Edge>> graph;

    explicit MinCostMaxFlow(int n) : n(n), graph(n) {}

    void add_edge(int u, int v, long long cap, long long cost) {
        Edge a{v, (int)graph[v].size(), cap, cost};
        Edge b{u, (int)graph[u].size(), 0, -cost};
        graph[u].push_back(a);
        graph[v].push_back(b);
    }

    pair<long long, long long> min_cost_max_flow(int s, int t) {
        const long long INF = (long long)4e18;
        long long flow = 0;
        long long cost = 0;
        vector<long long> potential(n, 0), dist(n);
        vector<int> parent_v(n), parent_e(n);

        while (true) {
            fill(dist.begin(), dist.end(), INF);
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
            dist[s] = 0;
            pq.push({0, s});

            while (!pq.empty()) {
                auto [d, v] = pq.top();
                pq.pop();
                if (d != dist[v]) continue;
                for (int i = 0; i < (int)graph[v].size(); ++i) {
                    const Edge& e = graph[v][i];
                    if (e.cap == 0) continue;
                    long long nd = d + e.cost + potential[v] - potential[e.to];
                    if (nd < dist[e.to]) {
                        dist[e.to] = nd;
                        parent_v[e.to] = v;
                        parent_e[e.to] = i;
                        pq.push({nd, e.to});
                    }
                }
            }

            if (dist[t] == INF) {
                break;
            }

            for (int v = 0; v < n; ++v) {
                if (dist[v] < INF) {
                    potential[v] += dist[v];
                }
            }

            long long add = INF;
            for (int v = t; v != s; v = parent_v[v]) {
                const Edge& e = graph[parent_v[v]][parent_e[v]];
                add = min(add, e.cap);
            }

            for (int v = t; v != s; v = parent_v[v]) {
                Edge& e = graph[parent_v[v]][parent_e[v]];
                e.cap -= add;
                graph[v][e.rev].cap += add;
                cost += add * e.cost;
            }

            flow += add;
        }

        return {flow, cost};
    }
};
