#include <bits/stdc++.h>
using namespace std;

namespace {

struct Node {
    char type = 0;
    char cond = 0;
    char proc = 0;
    int next = 0;
    int first = 0;
    int second = 0;
};

constexpr short INF_STATE = -1;
constexpr short UNVISITED = -2;
constexpr short IN_PROGRESS = -3;

int rows_count;
int cols_count;
vector<string> grid_cells;
vector<Node> nodes(1);
int proc_root[26];
vector<int> query_root;

int state_count;
vector<int> move_forward_state;
vector<int> turn_left_state;
vector<unsigned char> barrier_ahead;
vector<short> memo;

int encode_state(int r, int c, int dir) {
    return ((r * cols_count + c) << 2) | dir;
}

void decode_state(int state, int& r, int& c, int& dir) {
    dir = state & 3;
    state >>= 2;
    c = state % cols_count;
    r = state / cols_count;
}

bool check_condition(int state, char cond) {
    if (cond == 'b') {
        return barrier_ahead[state] != 0;
    }
    int dir = state & 3;
    if (cond == 'n') {
        return dir == 0;
    }
    if (cond == 'e') {
        return dir == 1;
    }
    if (cond == 's') {
        return dir == 2;
    }
    return dir == 3;
}

int parse_program(const string& s, int& pos);

int add_node(const Node& node) {
    nodes.push_back(node);
    return static_cast<int>(nodes.size()) - 1;
}

int parse_command_and_rest(const string& s, int& pos) {
    Node node;
    char ch = s[pos++];
    if (ch == 'm' || ch == 'l' || ('A' <= ch && ch <= 'Z')) {
        node.type = ch;
    } else if (ch == 'i') {
        node.type = 'i';
        node.cond = s[pos++];
        ++pos;  // '('
        node.first = parse_program(s, pos);
        ++pos;  // ')'
        ++pos;  // '('
        node.second = parse_program(s, pos);
        ++pos;  // ')'
    } else {
        node.type = 'u';
        node.cond = s[pos++];
        ++pos;  // '('
        node.first = parse_program(s, pos);
        ++pos;  // ')'
    }
    node.next = parse_program(s, pos);
    return add_node(node);
}

int parse_program(const string& s, int& pos) {
    if (pos >= static_cast<int>(s.size()) || s[pos] == ')') {
        return 0;
    }
    return parse_command_and_rest(s, pos);
}

int state_key(int node_id, int state) {
    return node_id * state_count + state;
}

short lookup_result(int node_id, int state) {
    if (node_id == 0) {
        return static_cast<short>(state);
    }
    return memo[state_key(node_id, state)];
}

short solve_state(int root_node, int root_state) {
    if (root_node == 0) {
        return static_cast<short>(root_state);
    }
    int root_key = state_key(root_node, root_state);
    if (memo[root_key] != UNVISITED) {
        return memo[root_key] == IN_PROGRESS ? INF_STATE : memo[root_key];
    }

    struct Frame {
        int node_id;
        int state;
        int stage;
        short temp;
    };

    vector<Frame> stack;
    stack.push_back({root_node, root_state, 0, 0});
    memo[root_key] = IN_PROGRESS;

    while (!stack.empty()) {
        Frame& frame = stack.back();
        int key = state_key(frame.node_id, frame.state);
        const Node& node = nodes[frame.node_id];

        auto request = [&](int child_node, int child_state, int next_stage) -> bool {
            short value = lookup_result(child_node, child_state);
            if (value == UNVISITED) {
                frame.stage = next_stage;
                memo[state_key(child_node, child_state)] = IN_PROGRESS;
                stack.push_back({child_node, child_state, 0, 0});
                return true;
            }
            if (value == IN_PROGRESS) {
                memo[key] = INF_STATE;
                stack.pop_back();
                return true;
            }
            frame.temp = value;
            frame.stage = next_stage;
            return false;
        };

        if (node.type == 'm') {
            if (frame.stage == 0) {
                if (request(node.next, move_forward_state[frame.state], 1)) {
                    continue;
                }
            }
            frame.temp = lookup_result(node.next, move_forward_state[frame.state]);
            memo[key] = frame.temp;
            stack.pop_back();
            continue;
        }

        if (node.type == 'l') {
            if (frame.stage == 0) {
                if (request(node.next, turn_left_state[frame.state], 1)) {
                    continue;
                }
            }
            frame.temp = lookup_result(node.next, turn_left_state[frame.state]);
            memo[key] = frame.temp;
            stack.pop_back();
            continue;
        }

        if ('A' <= node.type && node.type <= 'Z') {
            if (frame.stage == 0) {
                if (request(proc_root[node.type - 'A'], frame.state, 1)) {
                    continue;
                }
            }
            if (frame.stage == 1) {
                frame.temp = lookup_result(proc_root[node.type - 'A'], frame.state);
                if (frame.temp == INF_STATE) {
                    memo[key] = INF_STATE;
                    stack.pop_back();
                    continue;
                }
                if (request(node.next, frame.temp, 2)) {
                    continue;
                }
            }
            frame.temp = lookup_result(node.next, lookup_result(proc_root[node.type - 'A'], frame.state));
            memo[key] = frame.temp;
            stack.pop_back();
            continue;
        }

        if (node.type == 'i') {
            int branch = check_condition(frame.state, node.cond) ? node.first : node.second;
            if (frame.stage == 0) {
                if (request(branch, frame.state, 1)) {
                    continue;
                }
            }
            if (frame.stage == 1) {
                frame.temp = lookup_result(branch, frame.state);
                if (frame.temp == INF_STATE) {
                    memo[key] = INF_STATE;
                    stack.pop_back();
                    continue;
                }
                if (request(node.next, frame.temp, 2)) {
                    continue;
                }
            }
            frame.temp = lookup_result(node.next, lookup_result(branch, frame.state));
            memo[key] = frame.temp;
            stack.pop_back();
            continue;
        }

        if (frame.stage == 0) {
            if (check_condition(frame.state, node.cond)) {
                if (request(node.next, frame.state, 2)) {
                    continue;
                }
            } else {
                if (request(node.first, frame.state, 1)) {
                    continue;
                }
            }
        }
        if (frame.stage == 1) {
            frame.temp = lookup_result(node.first, frame.state);
            if (frame.temp == INF_STATE) {
                memo[key] = INF_STATE;
                stack.pop_back();
                continue;
            }
            if (request(frame.node_id, frame.temp, 2)) {
                continue;
            }
        }
        if (check_condition(frame.state, node.cond)) {
            frame.temp = lookup_result(node.next, frame.state);
        } else {
            frame.temp = lookup_result(frame.node_id, lookup_result(node.first, frame.state));
        }
        memo[key] = frame.temp;
        stack.pop_back();
    }

    return memo[root_key];
}

void build_transitions() {
    state_count = rows_count * cols_count * 4;
    move_forward_state.resize(state_count);
    turn_left_state.resize(state_count);
    barrier_ahead.resize(state_count);

    const int dr[4] = {-1, 0, 1, 0};
    const int dc[4] = {0, 1, 0, -1};
    for (int r = 0; r < rows_count; ++r) {
        for (int c = 0; c < cols_count; ++c) {
            for (int dir = 0; dir < 4; ++dir) {
                int state = encode_state(r, c, dir);
                turn_left_state[state] = encode_state(r, c, (dir + 3) & 3);

                int nr = r + dr[dir];
                int nc = c + dc[dir];
                bool blocked = nr < 0 || nr >= rows_count || nc < 0 || nc >= cols_count ||
                               grid_cells[nr][nc] == '#';
                barrier_ahead[state] = static_cast<unsigned char>(blocked);
                move_forward_state[state] = blocked ? state : encode_state(nr, nc, dir);
            }
        }
    }
}

void solve() {
    int definition_count, execution_count;
    cin >> rows_count >> cols_count >> definition_count >> execution_count;
    grid_cells.resize(rows_count);
    for (int i = 0; i < rows_count; ++i) {
        cin >> grid_cells[i];
    }

    fill(proc_root, proc_root + 26, 0);
    for (int i = 0; i < definition_count; ++i) {
        string line;
        cin >> line;
        int pos = 2;
        proc_root[line[0] - 'A'] = parse_program(line, pos);
    }

    vector<int> start_state(execution_count);
    query_root.resize(execution_count);
    for (int i = 0; i < execution_count; ++i) {
        int r, c;
        char heading;
        cin >> r >> c >> heading;
        string program;
        cin >> program;

        int dir = 0;
        if (heading == 'e') {
            dir = 1;
        } else if (heading == 's') {
            dir = 2;
        } else if (heading == 'w') {
            dir = 3;
        }
        start_state[i] = encode_state(r - 1, c - 1, dir);

        int pos = 0;
        query_root[i] = parse_program(program, pos);
    }

    build_transitions();
    memo.assign(static_cast<size_t>(nodes.size()) * state_count, UNVISITED);

    const char dir_name[4] = {'n', 'e', 's', 'w'};
    for (int i = 0; i < execution_count; ++i) {
        short result = solve_state(query_root[i], start_state[i]);
        if (result == INF_STATE) {
            cout << "inf\n";
            continue;
        }

        int r, c, dir;
        decode_state(result, r, c, dir);
        cout << (r + 1) << ' ' << (c + 1) << ' ' << dir_name[dir] << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
