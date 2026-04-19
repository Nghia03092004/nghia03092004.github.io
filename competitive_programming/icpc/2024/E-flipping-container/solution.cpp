#include <bits/stdc++.h>
using namespace std;

namespace {

int gcd_int(int a, int b) {
    while (b != 0) {
        const int r = a % b;
        a = b;
        b = r;
    }
    return abs(a);
}

struct Solver1D {
    static constexpr long long kImpossible = (1LL << 60);

    int gcd_value = 0;
    vector<int> moves;
    int best_move = 0;
    int limit = 0;
    vector<int> exact;
    vector<int> anchor;

    Solver1D() = default;

        explicit Solver1D(const vector<int>& raw_moves) {
        for (const int value : raw_moves) {
            gcd_value = gcd_int(gcd_value, value);
        }

        for (const int value : raw_moves) {
            moves.push_back(value / gcd_value);
        }
        sort(moves.begin(), moves.end());
        moves.erase(unique(moves.begin(), moves.end()), moves.end());
        best_move = moves.back();

        if (moves.size() == 1) {
            exact = {0};
            anchor = {0};
            return;
        }

        limit = best_move * (best_move - 1);
        const int bfs_limit = limit + best_move;

        vector<int> dist(bfs_limit + 1, -1);
        vector<int> queue;
        queue.reserve(bfs_limit + 1);
        dist[0] = 0;
        queue.push_back(0);

        for (size_t head = 0; head < queue.size(); ++head) {
            const int current = queue[head];
            const int next_dist = dist[current] + 1;
            for (const int move : moves) {
                const int down = abs(current - move);
                if (dist[down] == -1) {
                    dist[down] = next_dist;
                    queue.push_back(down);
                }

                const int up = current + move;
                if (up <= bfs_limit && dist[up] == -1) {
                    dist[up] = next_dist;
                    queue.push_back(up);
                }
            }
        }

        exact.assign(dist.begin(), dist.begin() + limit + 1);
        anchor.assign(best_move, -1);
        vector<long long> best_reduced(best_move, kImpossible);

        for (int value = 0; value <= limit; ++value) {
            if (exact[value] == -1) {
                continue;
            }
            const int residue = value % best_move;
            const long long reduced =
                1LL * exact[value] * best_move - value;
            if (reduced < best_reduced[residue] ||
                (reduced == best_reduced[residue] &&
                 (anchor[residue] == -1 || value < anchor[residue]))) {
                best_reduced[residue] = reduced;
                anchor[residue] = value;
            }
        }
    }

    long long query(long long target) const {
        target = llabs(target);
        if (target % gcd_value != 0) {
            return kImpossible;
        }

        target /= gcd_value;
        if (moves.size() == 1) {
            const int move = moves[0];
            return (target % move == 0) ? (target / move) : kImpossible;
        }

        if (target <= limit) {
            const int answer = exact[static_cast<int>(target)];
            return (answer == -1) ? kImpossible : answer;
        }

        const int residue = static_cast<int>(target % best_move);
        const int start = anchor[residue];
        if (start == -1 || target < start) {
            return kImpossible;
        }
        return exact[start] + (target - start) / best_move;
    }
};

struct Step {
    int axis = 0;  // 0 = x, 1 = y
    int length = 0;
};

struct Walk {
    vector<Step> steps;
    int x_mask = 0;
    int y_mask = 0;
};

vector<Walk> build_walks(const array<int, 6>& state_x_id,
                         const array<int, 6>& state_y_id,
                         const array<int, 6>& edge_axis,
                         const array<int, 6>& edge_length_id) {
    vector<Walk> walks;

    auto add_walk = [&](const vector<int>& path) {
        Walk walk;
        int visited_states = 1;  // state 0
        int current = 0;
        for (const int next : path) {
            visited_states |= (1 << next);
            const int edge =
                (next == (current + 1) % 6) ? current : next;
            walk.steps.push_back({edge_axis[edge], edge_length_id[edge]});
            current = next;
        }

        for (int state = 0; state < 6; ++state) {
            if ((visited_states & (1 << state)) == 0) {
                continue;
            }
            walk.x_mask |= 1 << state_x_id[state];
            walk.y_mask |= 1 << state_y_id[state];
        }
        walks.push_back(std::move(walk));
    };

    vector<int> path;
    bool used[6][6] = {};

    function<void(int)> dfs = [&](const int current) {
        if (current == 0) {
            add_walk(path);
        }

        for (const int next : array<int, 2>{(current + 5) % 6, (current + 1) % 6}) {
            if (used[current][next]) {
                continue;
            }
            used[current][next] = true;
            path.push_back(next);
            dfs(next);
            path.pop_back();
            used[current][next] = false;
        }
    };

    add_walk({});
    dfs(0);
    return walks;
}

void solve() {
    long long a;
    long long b;
    long long c;
    cin >> a >> b >> c;

    long long target_x;
    long long target_y;
    cin >> target_x >> target_y;

    const array<long long, 3> pair_length = {a + b, a + c, b + c};

    const array<int, 6> state_x_id = {1, 0, 0, 2, 2, 1};
    const array<int, 6> state_y_id = {2, 2, 1, 1, 0, 0};

    const array<int, 6> edge_axis = {1, 0, 1, 0, 1, 0};
    const array<int, 6> edge_length_id = {2, 0, 1, 2, 0, 1};
    const vector<Walk> walks =
        build_walks(state_x_id, state_y_id, edge_axis, edge_length_id);

    array<unique_ptr<Solver1D>, 8> solver_cache;
    auto get_solver = [&](const int mask) -> const Solver1D& {
        if (!solver_cache[mask]) {
            vector<int> raw_moves;
            for (int id = 0; id < 3; ++id) {
                if (mask & (1 << id)) {
                    raw_moves.push_back(
                        static_cast<int>(pair_length[id]));
                }
            }
            solver_cache[mask] = make_unique<Solver1D>(raw_moves);
        }
        return *solver_cache[mask];
    };

    const long long doubled_target_x = 2 * target_x;
    const long long doubled_target_y = 2 * target_y;
    long long answer = Solver1D::kImpossible;

    for (const Walk& walk : walks) {
        const Solver1D& x_solver = get_solver(walk.x_mask);
        const Solver1D& y_solver = get_solver(walk.y_mask);

        const int step_count = static_cast<int>(walk.steps.size());
        const int sign_masks = 1 << step_count;
        for (int mask = 0; mask < sign_masks; ++mask) {
            long long base_x = 0;
            long long base_y = 0;
            for (int i = 0; i < step_count; ++i) {
                const long long sign = ((mask >> i) & 1) ? 1 : -1;
                if (walk.steps[i].axis == 0) {
                    base_x += sign * pair_length[walk.steps[i].length];
                } else {
                    base_y += sign * pair_length[walk.steps[i].length];
                }
            }

            const long long remain_x = doubled_target_x - base_x;
            const long long remain_y = doubled_target_y - base_y;
            if (remain_x % 2 != 0 || remain_y % 2 != 0) {
                continue;
            }

            const long long extra_x = x_solver.query(remain_x / 2);
            if (extra_x == Solver1D::kImpossible) {
                continue;
            }

            const long long extra_y = y_solver.query(remain_y / 2);
            if (extra_y == Solver1D::kImpossible) {
                continue;
            }

            answer = min(answer,
                         static_cast<long long>(step_count) +
                             2 * extra_x + 2 * extra_y);
        }
    }

    if (answer == Solver1D::kImpossible) {
        cout << "impossible\n";
    } else {
        cout << answer << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
