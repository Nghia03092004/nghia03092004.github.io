#include <bits/stdc++.h>
using namespace std;

namespace {

struct Point {
    int r = 0;
    int c = 0;

    bool operator==(const Point& other) const {
        return r == other.r && c == other.c;
    }

    bool operator<(const Point& other) const {
        if (r != other.r) {
            return r < other.r;
        }
        return c < other.c;
    }
};

Point operator+(const Point& a, const Point& b) {
    return {a.r + b.r, a.c + b.c};
}

Point rotate_point(const Point& p, const int rot) {
    if (rot == 0) {
        return p;
    }
    if (rot == 1) {
        return {-p.c, p.r};
    }
    if (rot == 2) {
        return {-p.r, -p.c};
    }
    return {p.c, -p.r};
}

constexpr array<Point, 4> kDirDelta = {{
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1},
}};

int rows;
int cols;
vector<string> grid;
int current_distance = 0;
int orientation = 0;

vector<Point> local_cells;
vector<array<int, 4>> graph_dirs;
map<pair<int, int>, int> id_of;

void read_distance() {
    cin >> current_distance;
}

void issue(const string& cmd) {
    cout << cmd << '\n';
    cout.flush();
    if (cmd == "no" || cmd.substr(0, 3) == "yes") {
        return;
    }
    read_distance();
}

void turn_right() {
    orientation = (orientation + 1) % 4;
    issue("right");
}

void turn_left() {
    orientation = (orientation + 3) % 4;
    issue("left");
}

void turn_to(const int target_dir) {
    int diff = (target_dir - orientation + 4) % 4;
    if (diff == 3) {
        turn_left();
    } else {
        while (diff-- > 0) {
            turn_right();
        }
    }
}

void step_forward() {
    issue("step");
}

pair<int, bool> get_or_create_id(const Point& p) {
    const pair<int, int> key = {p.r, p.c};
    auto it = id_of.find(key);
    if (it != id_of.end()) {
        return {it->second, false};
    }
    const int id = static_cast<int>(local_cells.size());
    id_of[key] = id;
    local_cells.push_back(p);
    graph_dirs.push_back({-1, -1, -1, -1});
    return {id, true};
}

void explore(const int id) {
    const Point cell = local_cells[id];
    for (int step = 0; step < 4; ++step) {
        const int dir = orientation;
        const Point next = cell + kDirDelta[dir];
        if (current_distance > 0) {
            const pair<int, bool> created = get_or_create_id(next);
            const int next_id = created.first;
            const bool is_new = created.second;
            graph_dirs[id][dir] = next_id;
            graph_dirs[next_id][(dir + 2) % 4] = id;
            if (is_new) {
                step_forward();
                explore(next_id);
                turn_right();
                turn_right();
                step_forward();
                turn_right();
                turn_right();
            }
        }
        turn_right();
    }
}

struct Transformation {
    int rot = 0;
    Point shift;
};

vector<Point> normalize(vector<Point> pts, Point& min_point) {
    min_point = *min_element(pts.begin(), pts.end());
    for (Point& p : pts) {
        p.r -= min_point.r;
        p.c -= min_point.c;
    }
    sort(pts.begin(), pts.end());
    return pts;
}

void solve() {
    cin >> rows >> cols;
    grid.resize(rows);
    for (string& row : grid) {
        cin >> row;
    }

    read_distance();

    get_or_create_id({0, 0});
    explore(0);

    vector<Point> component_cells = local_cells;

    vector<vector<Point>> map_components;
    vector<vector<int>> comp_id(rows, vector<int>(cols, -1));
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (grid[r][c] == '#' || comp_id[r][c] != -1) {
                continue;
            }
            const int cid = static_cast<int>(map_components.size());
            queue<Point> q;
            q.push({r, c});
            comp_id[r][c] = cid;
            map_components.push_back({});
            while (!q.empty()) {
                const Point cur = q.front();
                q.pop();
                map_components.back().push_back(cur);
                for (const Point& delta : kDirDelta) {
                    const int nr = cur.r + delta.r;
                    const int nc = cur.c + delta.c;
                    if (nr < 0 || nr >= rows || nc < 0 || nc >= cols) {
                        continue;
                    }
                    if (grid[nr][nc] == '#' || comp_id[nr][nc] != -1) {
                        continue;
                    }
                    comp_id[nr][nc] = cid;
                    q.push({nr, nc});
                }
            }
        }
    }

    vector<Transformation> transforms;
    for (const vector<Point>& comp : map_components) {
        if (comp.size() != component_cells.size()) {
            continue;
        }
        Point comp_min;
        const vector<Point> comp_norm = normalize(comp, comp_min);
        for (int rot = 0; rot < 4; ++rot) {
            vector<Point> rotated;
            rotated.reserve(component_cells.size());
            for (const Point& p : component_cells) {
                rotated.push_back(rotate_point(p, rot));
            }
            Point rot_min;
            const vector<Point> rot_norm = normalize(rotated, rot_min);
            if (rot_norm == comp_norm) {
                transforms.push_back({rot, {comp_min.r - rot_min.r,
                                            comp_min.c - rot_min.c}});
            }
        }
    }

    if (transforms.empty()) {
        issue("no");
        return;
    }

    int target_local = -1;
    Point target_global;
    for (int id = 0; id < static_cast<int>(component_cells.size()); ++id) {
        const Point local = component_cells[id];
        const Point first_image =
            rotate_point(local, transforms[0].rot) + transforms[0].shift;
        bool same = true;
        for (const Transformation& t : transforms) {
            const Point image = rotate_point(local, t.rot) + t.shift;
            if (!(image == first_image)) {
                same = false;
                break;
            }
        }
        if (same) {
            target_local = id;
            target_global = first_image;
            break;
        }
    }

    if (target_local == -1) {
        issue("no");
        return;
    }

    vector<int> parent(local_cells.size(), -1);
    vector<int> parent_dir(local_cells.size(), -1);
    queue<int> q;
    q.push(0);
    parent[0] = 0;
    while (!q.empty()) {
        const int u = q.front();
        q.pop();
        if (u == target_local) {
            break;
        }
        for (int dir = 0; dir < 4; ++dir) {
            const int v = graph_dirs[u][dir];
            if (v == -1 || parent[v] != -1) {
                continue;
            }
            parent[v] = u;
            parent_dir[v] = dir;
            q.push(v);
        }
    }

    vector<int> path_dirs;
    for (int cur = target_local; cur != 0; cur = parent[cur]) {
        path_dirs.push_back(parent_dir[cur]);
    }
    reverse(path_dirs.begin(), path_dirs.end());

    for (const int dir : path_dirs) {
        turn_to(dir);
        step_forward();
    }

    ostringstream out;
    out << "yes " << (target_global.r + 1) << ' ' << (target_global.c + 1);
    issue(out.str());
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
