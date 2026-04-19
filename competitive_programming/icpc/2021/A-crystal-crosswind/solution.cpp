#include <bits/stdc++.h>
using namespace std;

namespace {

struct Wind {
    int wx;
    int wy;
    vector<unsigned char> boundary;
};

int width;
int height;
int cell_count;
int wind_count;
vector<Wind> winds;

inline bool in_bounds(int x, int y) {
    return 0 <= x && x < width && 0 <= y && y < height;
}

inline int id_of(int x, int y) {
    return y * width + x;
}

vector<unsigned char> build_false_seeds() {
    vector<unsigned char> forced_false(cell_count, 0);

    for (const Wind& wind : winds) {
        for (int id = 0; id < cell_count; ++id) {
            if (!wind.boundary[id]) {
                int x = id % width;
                int y = id / width;
                int px = x - wind.wx;
                int py = y - wind.wy;
                if (!in_bounds(px, py)) {
                    forced_false[id] = 1;
                }
            }
        }

        for (int id = 0; id < cell_count; ++id) {
            if (!wind.boundary[id]) {
                continue;
            }
            int x = id % width;
            int y = id / width;
            int px = x - wind.wx;
            int py = y - wind.wy;
            if (in_bounds(px, py)) {
                forced_false[id_of(px, py)] = 1;
            }
        }
    }

    return forced_false;
}

vector<unsigned char> build_true_seeds() {
    vector<unsigned char> forced_true(cell_count, 0);
    for (const Wind& wind : winds) {
        for (int id = 0; id < cell_count; ++id) {
            if (wind.boundary[id]) {
                forced_true[id] = 1;
            }
        }
    }
    return forced_true;
}

vector<unsigned char> build_minimal_structure(
    const vector<unsigned char>& forced_true,
    const vector<unsigned char>& forced_false
) {
    vector<unsigned char> present(cell_count, 0);
    vector<int> queue;
    queue.reserve(cell_count);

    for (int id = 0; id < cell_count; ++id) {
        if (forced_true[id]) {
            present[id] = 1;
            queue.push_back(id);
        }
    }

    for (size_t head = 0; head < queue.size(); ++head) {
        int id = queue[head];
        int x = id % width;
        int y = id / width;

        for (const Wind& wind : winds) {
            if (wind.boundary[id]) {
                continue;
            }
            int px = x - wind.wx;
            int py = y - wind.wy;
            if (!in_bounds(px, py)) {
                continue;
            }
            int prev = id_of(px, py);
            if (forced_false[prev] || present[prev]) {
                continue;
            }
            present[prev] = 1;
            queue.push_back(prev);
        }
    }

    return present;
}

vector<unsigned char> build_maximal_structure(const vector<unsigned char>& forced_false) {
    vector<unsigned char> absent = forced_false;
    vector<int> queue;
    queue.reserve(cell_count);

    for (int id = 0; id < cell_count; ++id) {
        if (absent[id]) {
            queue.push_back(id);
        }
    }

    for (size_t head = 0; head < queue.size(); ++head) {
        int id = queue[head];
        int x = id % width;
        int y = id / width;

        for (const Wind& wind : winds) {
            int nx = x + wind.wx;
            int ny = y + wind.wy;
            if (!in_bounds(nx, ny)) {
                continue;
            }
            int next = id_of(nx, ny);
            if (wind.boundary[next] || absent[next]) {
                continue;
            }
            absent[next] = 1;
            queue.push_back(next);
        }
    }

    vector<unsigned char> present(cell_count, 1);
    for (int id = 0; id < cell_count; ++id) {
        if (absent[id]) {
            present[id] = 0;
        }
    }
    return present;
}

void print_structure(const vector<unsigned char>& present) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cout << (present[id_of(x, y)] ? '#' : '.');
        }
        cout << '\n';
    }
}

void solve() {
    cin >> width >> height >> wind_count;
    cell_count = width * height;
    winds.clear();
    winds.reserve(wind_count);

    for (int i = 0; i < wind_count; ++i) {
        Wind wind;
        cin >> wind.wx >> wind.wy;
        wind.boundary.assign(cell_count, 0);

        int b;
        cin >> b;
        for (int j = 0; j < b; ++j) {
            int x, y;
            cin >> x >> y;
            --x;
            --y;
            wind.boundary[id_of(x, y)] = 1;
        }
        winds.push_back(move(wind));
    }

    vector<unsigned char> forced_true = build_true_seeds();
    vector<unsigned char> forced_false = build_false_seeds();

    vector<unsigned char> minimal = build_minimal_structure(forced_true, forced_false);
    vector<unsigned char> maximal = build_maximal_structure(forced_false);

    print_structure(minimal);
    cout << '\n';
    print_structure(maximal);
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
