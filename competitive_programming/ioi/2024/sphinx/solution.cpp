#include <algorithm>
#include <functional>
#include <numeric>
#include <queue>
#include <vector>

using namespace std;

int perform_experiment(vector<int> E);

namespace {

int count_components(int N, const vector<vector<int>>& graph, const vector<int>& color) {
    int components = 0;
    vector<bool> visited(N, false);
    queue<int> q;

    for (int start = 0; start < N; ++start) {
        if (visited[start]) {
            continue;
        }
        ++components;
        visited[start] = true;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v : graph[u]) {
                if (!visited[v] && color[v] == color[u]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    return components;
}

vector<vector<int>> find_monochromatic_components(int N, const vector<vector<int>>& graph) {
    vector<vector<int>> components = {{0}};
    vector<int> order(N), color(N);

    for (int u = 1; u < N; ++u) {
        order.assign(N, N);
        color.assign(N, N);
        order[u] = color[u] = -1;

        int current_components = static_cast<int>(components.size());
        for (int i = 0; i < current_components; ++i) {
            for (int v : components[i]) {
                order[v] = -1;
                color[v] = i;
            }
        }

        int expected = count_components(N, graph, color);
        int merges = expected - perform_experiment(order);
        if (merges == 0) {
            components.push_back({u});
            continue;
        }

        int lo = 0;
        int hi = current_components;
        vector<int> to_merge;

        while (merges > 0) {
            while (lo + 1 < hi) {
                int mid = (lo + hi) / 2;
                order.assign(N, N);
                color.assign(N, N);
                order[u] = color[u] = -1;
                for (int i = mid; i < hi; ++i) {
                    for (int v : components[i]) {
                        order[v] = -1;
                        color[v] = i;
                    }
                }
                if (perform_experiment(order) < count_components(N, graph, color)) {
                    lo = mid;
                } else {
                    hi = mid;
                }
            }
            to_merge.push_back(lo);
            lo = 0;
            --hi;
            --merges;
        }

        int keep = to_merge.back();
        to_merge.pop_back();
        for (int idx : to_merge) {
            for (int v : components[idx]) {
                components[keep].push_back(v);
            }
            components.erase(components.begin() + idx);
        }
        components[keep].push_back(u);
    }

    return components;
}

void assign_colours(const vector<int>& side, vector<int>& component_colour, int N,
                    const vector<vector<int>>& graph, const vector<vector<int>>& components) {
    vector<int> order(N), color(N);

    for (int real_colour = 0; real_colour < N; ++real_colour) {
        int lo = 0;
        int hi = static_cast<int>(side.size());

        while (true) {
            order.assign(N, real_colour);
            color.assign(N, N);
            for (int i = lo; i < hi; ++i) {
                for (int u : components[side[i]]) {
                    order[u] = -1;
                    color[u] = i;
                }
            }

            int matches = count_components(N, graph, color) - perform_experiment(order);
            if (matches == 0) {
                break;
            }

            while (lo + 1 < hi) {
                int mid = (lo + hi) / 2;
                order.assign(N, real_colour);
                color.assign(N, N);
                for (int i = mid; i < hi; ++i) {
                    for (int u : components[side[i]]) {
                        order[u] = -1;
                        color[u] = i;
                    }
                }
                if (perform_experiment(order) < count_components(N, graph, color)) {
                    lo = mid;
                } else {
                    hi = mid;
                }
            }

            component_colour[side[lo]] = real_colour;
            lo = 0;
            --hi;
            if (matches == 1) {
                break;
            }
        }
    }
}

}  // namespace

vector<int> find_colours(int N, vector<int> X, vector<int> Y) {
    vector<vector<int>> graph(N);
    for (int i = 0; i < static_cast<int>(X.size()); ++i) {
        graph[X[i]].push_back(Y[i]);
        graph[Y[i]].push_back(X[i]);
    }

    vector<vector<int>> components = find_monochromatic_components(N, graph);
    int component_count = static_cast<int>(components.size());

    if (component_count == 1) {
        for (int real_colour = 0; real_colour < N; ++real_colour) {
            vector<int> order(N, real_colour);
            order[0] = -1;
            if (perform_experiment(order) == 1) {
                return vector<int>(N, real_colour);
            }
        }
    }

    vector<int> component_id(N, -1);
    for (int c = 0; c < component_count; ++c) {
        for (int u : components[c]) {
            component_id[u] = c;
        }
    }

    vector<vector<int>> collapsed(component_count, vector<int>(component_count, 0));
    for (int i = 0; i < static_cast<int>(X.size()); ++i) {
        int a = component_id[X[i]];
        int b = component_id[Y[i]];
        if (a != b) {
            collapsed[a][b] = collapsed[b][a] = 1;
        }
    }

    vector<int> side(component_count, 0);
    queue<int> q;
    for (int start = 0; start < component_count; ++start) {
        if (side[start] != 0) {
            continue;
        }
        side[start] = 1;
        q.push(start);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            for (int v = 0; v < component_count; ++v) {
                if (collapsed[u][v] && side[v] == 0) {
                    side[v] = (side[u] == 1 ? 2 : 1);
                    q.push(v);
                }
            }
        }
    }

    vector<int> left, right;
    for (int c = 0; c < component_count; ++c) {
        if (side[c] == 1) {
            left.push_back(c);
        } else {
            right.push_back(c);
        }
    }

    vector<int> component_colour(component_count, -1);
    assign_colours(left, component_colour, N, graph, components);
    assign_colours(right, component_colour, N, graph, components);

    vector<int> answer(N);
    for (int u = 0; u < N; ++u) {
        answer[u] = component_colour[component_id[u]];
    }
    return answer;
}
