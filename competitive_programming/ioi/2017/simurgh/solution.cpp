#include <bits/stdc++.h>
using namespace std;

int count_common_roads(vector<int> &r);

namespace {

struct Solver {
    int n, m;
    vector<int> U, V;
    vector<vector<int>> adj;
    vector<int> tin, low, comp_id;
    vector<char> is_bridge;
    int timer = 0;
    vector<int> status;
    vector<vector<int>> comp_edges;
    vector<vector<int>> comp_vertices;
    vector<char> in_sub;
    struct Ear {
        int a = -1;
        int b = -1;
        vector<int> edges;
    };

    int other(int eid, int x) const { return U[eid] ^ V[eid] ^ x; }

    void bridge_dfs(int v, int peid) {
        tin[v] = low[v] = timer++;
        for (int eid : adj[v]) {
            if (eid == peid) continue;
            int to = other(eid, v);
            if (tin[to] != -1) {
                low[v] = min(low[v], tin[to]);
            } else {
                bridge_dfs(to, eid);
                low[v] = min(low[v], low[to]);
                if (low[to] > tin[v]) is_bridge[eid] = 1;
            }
        }
    }

    void build_components() {
        tin.assign(n, -1);
        low.assign(n, 0);
        is_bridge.assign(m, 0);
        bridge_dfs(0, -1);

        status.assign(m, -1);
        for (int eid = 0; eid < m; ++eid) {
            if (is_bridge[eid]) status[eid] = 1;
        }

        comp_id.assign(n, -1);
        int comps = 0;
        for (int s = 0; s < n; ++s) {
            if (comp_id[s] != -1) continue;
            queue<int> q;
            q.push(s);
            comp_id[s] = comps;
            while (!q.empty()) {
                int v = q.front();
                q.pop();
                for (int eid : adj[v]) {
                    if (is_bridge[eid]) continue;
                    int to = other(eid, v);
                    if (comp_id[to] == -1) {
                        comp_id[to] = comps;
                        q.push(to);
                    }
                }
            }
            ++comps;
        }

        comp_edges.assign(comps, {});
        comp_vertices.assign(comps, {});
        for (int v = 0; v < n; ++v) comp_vertices[comp_id[v]].push_back(v);
        for (int eid = 0; eid < m; ++eid) {
            if (is_bridge[eid]) continue;
            comp_edges[comp_id[U[eid]]].push_back(eid);
        }
    }

    vector<int> build_query_tree(const vector<int> &cycle_edges, int omit) {
        vector<char> blocked(m, 0), seen(n, 0);
        for (int eid : cycle_edges) blocked[eid] = 1;

        queue<int> q;
        for (int eid : cycle_edges) {
            if (!seen[U[eid]]) {
                seen[U[eid]] = 1;
                q.push(U[eid]);
            }
            if (!seen[V[eid]]) {
                seen[V[eid]] = 1;
                q.push(V[eid]);
            }
        }

        vector<int> tree_edges;
        tree_edges.reserve(n - 1);
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int eid : adj[v]) {
                if (blocked[eid]) continue;
                int to = other(eid, v);
                if (!seen[to]) {
                    seen[to] = 1;
                    q.push(to);
                    tree_edges.push_back(eid);
                }
            }
        }

        for (int eid : cycle_edges) {
            if (eid != omit) tree_edges.push_back(eid);
        }
        return tree_edges;
    }

    vector<int> path_without_edge(int start, int goal, int banned, int cid) {
        vector<int> parent_v(n, -1), parent_e(n, -1);
        queue<int> q;
        q.push(start);
        parent_v[start] = start;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (v == goal) break;
            for (int eid : adj[v]) {
                if (eid == banned || is_bridge[eid]) continue;
                if (comp_id[other(eid, v)] != cid) continue;
                int to = other(eid, v);
                if (parent_v[to] != -1) continue;
                parent_v[to] = v;
                parent_e[to] = eid;
                q.push(to);
            }
        }

        vector<int> path;
        int cur = goal;
        while (cur != start) {
            path.push_back(parent_e[cur]);
            cur = parent_v[cur];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    void classify_initial_cycle(const vector<int> &cycle) {
        vector<int> values(cycle.size());
        int best = -1;
        for (int i = 0; i < (int)cycle.size(); ++i) {
            vector<int> tree = build_query_tree(cycle, cycle[i]);
            values[i] = count_common_roads(tree);
            best = max(best, values[i]);
        }
        for (int i = 0; i < (int)cycle.size(); ++i) {
            status[cycle[i]] = (values[i] < best ? 1 : 0);
        }
    }

    vector<int> known_path(int src, int dst, int cid) {
        vector<int> parent_v(n, -1), parent_e(n, -1);
        queue<int> q;
        q.push(src);
        parent_v[src] = src;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            if (v == dst) break;
            for (int eid : adj[v]) {
                if (is_bridge[eid] || status[eid] == -1) continue;
                if (comp_id[other(eid, v)] != cid) continue;
                int to = other(eid, v);
                if (parent_v[to] != -1) continue;
                parent_v[to] = v;
                parent_e[to] = eid;
                q.push(to);
            }
        }

        vector<int> path;
        int cur = dst;
        while (cur != src) {
            path.push_back(parent_e[cur]);
            cur = parent_v[cur];
        }
        reverse(path.begin(), path.end());
        return path;
    }

    vector<int> reconstruct_to_source(int v, const vector<int> &par_v,
                                      const vector<int> &par_e, int source) {
        vector<int> path;
        while (v != source) {
            path.push_back(par_e[v]);
            v = par_v[v];
        }
        return path;
    }

    Ear find_ear(int cid) {
        vector<int> source(n, -1), par_v(n, -1), par_e(n, -1);
        queue<int> q;
        for (int v : comp_vertices[cid]) {
            if (in_sub[v]) {
                source[v] = v;
                q.push(v);
            }
        }

        auto build = [&](int end_a, int end_b, int meet_u, int meet_v, int bridge_e) {
            vector<int> left = reconstruct_to_source(meet_u, par_v, par_e, end_a);
            reverse(left.begin(), left.end());
            vector<int> right = reconstruct_to_source(meet_v, par_v, par_e, end_b);
            vector<int> ear = left;
            if (bridge_e != -1) ear.push_back(bridge_e);
            ear.insert(ear.end(), right.begin(), right.end());
            return Ear{end_a, end_b, ear};
        };

        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (int eid : adj[v]) {
                if (is_bridge[eid] || status[eid] != -1) continue;
                int to = other(eid, v);
                if (comp_id[to] != cid) continue;

                if (in_sub[v] && in_sub[to] && source[v] != source[to]) {
                    return Ear{v, to, {eid}};
                }

                if (!in_sub[to]) {
                    if (source[to] == -1) {
                        source[to] = source[v];
                        par_v[to] = v;
                        par_e[to] = eid;
                        q.push(to);
                    } else if (source[to] != source[v]) {
                        int s1 = source[v];
                        int s2 = source[to];
                        return build(s1, s2, v, to, eid);
                    } else if (par_v[v] != to && par_v[to] != v) {
                        int s = source[v];
                        return build(s, s, v, to, eid);
                    }
                } else if (source[v] != to) {
                    int s1 = source[v];
                    int s2 = to;
                    return build(s1, s2, v, to, eid);
                } else if (!in_sub[v] && par_v[v] != to) {
                    int s = source[v];
                    return build(s, s, v, v, eid);
                }
            }
        }
        return Ear{};
    }

    void classify_ear(const Ear &ear, int cid) {
        int a = ear.a;
        int b = ear.b;
        vector<int> q_path = known_path(a, b, cid);
        vector<int> cycle = q_path;
        cycle.insert(cycle.end(), ear.edges.begin(), ear.edges.end());

        int baseline_nonroyal = -1;
        for (int eid : q_path) {
            if (status[eid] == 0) {
                baseline_nonroyal = eid;
                break;
            }
        }

        if (baseline_nonroyal != -1) {
            vector<int> base_tree = build_query_tree(cycle, baseline_nonroyal);
            int base = count_common_roads(base_tree);
            for (int eid : ear.edges) {
                vector<int> tree = build_query_tree(cycle, eid);
                int val = count_common_roads(tree);
                status[eid] = (val < base ? 1 : 0);
            }
        } else {
            vector<int> values(ear.edges.size());
            int best = -1;
            for (int i = 0; i < (int)ear.edges.size(); ++i) {
                vector<int> tree = build_query_tree(cycle, ear.edges[i]);
                values[i] = count_common_roads(tree);
                best = max(best, values[i]);
            }
            for (int i = 0; i < (int)ear.edges.size(); ++i) {
                status[ear.edges[i]] = (values[i] < best ? 1 : 0);
            }
        }

        for (int eid : ear.edges) {
            in_sub[U[eid]] = 1;
            in_sub[V[eid]] = 1;
        }
    }

    vector<int> solve_component(int cid) {
        if (comp_edges[cid].empty()) return {};

        in_sub.assign(n, 0);
        int first = comp_edges[cid][0];
        vector<int> cycle = path_without_edge(U[first], V[first], first, cid);
        cycle.push_back(first);
        classify_initial_cycle(cycle);
        for (int eid : cycle) {
            in_sub[U[eid]] = 1;
            in_sub[V[eid]] = 1;
        }

        while (true) {
            bool done = true;
            for (int eid : comp_edges[cid]) {
                if (status[eid] == -1) {
                    done = false;
                    break;
                }
            }
            if (done) break;
            Ear ear = find_ear(cid);
            if (ear.edges.empty()) break;
            classify_ear(ear, cid);
        }

        vector<int> royal;
        for (int eid : comp_edges[cid]) {
            if (status[eid] == 1) royal.push_back(eid);
        }
        return royal;
    }

    vector<int> run() {
        build_components();
        for (int cid = 0; cid < (int)comp_edges.size(); ++cid) {
            solve_component(cid);
        }
        vector<int> answer;
        for (int eid = 0; eid < m; ++eid) if (status[eid] == 1) answer.push_back(eid);
        sort(answer.begin(), answer.end());
        return answer;
    }
};

}  // namespace

vector<int> find_roads(int n, vector<int> u, vector<int> v) {
    Solver solver;
    solver.n = n;
    solver.m = (int)u.size();
    solver.U = move(u);
    solver.V = move(v);
    solver.adj.assign(n, {});
    for (int i = 0; i < solver.m; ++i) {
        solver.adj[solver.U[i]].push_back(i);
        solver.adj[solver.V[i]].push_back(i);
    }
    return solver.run();
}

int main() { return 0; }
