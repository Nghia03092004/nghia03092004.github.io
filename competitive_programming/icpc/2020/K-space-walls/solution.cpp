#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr long long INF64 = (1LL << 62);

struct AxisDir {
    int axis;
    int sign;
};

struct Box {
    int lo[3];
    int hi[3];
};

struct RobotInput {
    int cube[3];
    AxisDir face;
    AxisDir move;
};

long long mod_norm(long long x, long long mod) {
    x %= mod;
    if (x < 0) {
        x += mod;
    }
    return x;
}

long long ext_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return llabs(a);
    }
    long long x1 = 0, y1 = 0;
    long long g = ext_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

bool solve_linear_congruence(long long a, long long b, long long mod, long long& result) {
    a = mod_norm(a, mod);
    b = mod_norm(b, mod);
    long long x = 0, y = 0;
    long long g = ext_gcd(a, mod, x, y);
    if (b % g != 0) {
        return false;
    }
    long long a2 = a / g;
    long long b2 = b / g;
    long long mod2 = mod / g;
    long long inv = mod_norm(x, mod2);
    (void)a2;
    result = mod_norm(inv * b2, mod2);
    return true;
}

bool crt_two(long long r1, long long m1, long long r2, long long m2, long long& result) {
    if (m1 == 0 || m2 == 0) {
        return false;
    }
    long long x = 0, y = 0;
    long long g = ext_gcd(m1, m2, x, y);
    long long diff = r2 - r1;
    if (diff % g != 0) {
        return false;
    }
    long long mod2 = m2 / g;
    long long mult = (diff / g) * mod_norm(x, mod2);
    long long k = mult % mod2;
    if (k < 0) {
        k += mod2;
    }
    long long result128 = r1 + m1 * k;
    long long lcm = m1 / g * m2;
    long long ans = result128 % lcm;
    if (ans < 0) {
        ans += lcm;
    }
    result = ans;
    return true;
}

AxisDir parse_axis_dir(const string& s) {
    AxisDir out{};
    if (s[0] == 'x') {
        out.axis = 0;
    } else if (s[0] == 'y') {
        out.axis = 1;
    } else {
        out.axis = 2;
    }
    out.sign = (s[1] == '+') ? 1 : -1;
    return out;
}

struct Slice {
    struct PrimitiveEdge {
        int sx = 0;
        int sy = 0;
        int dir = 0;
        long long len = 0;
        int normal_axis = -1;
        int normal_sign = 0;
    };

    struct Segment {
        int dir = 0;
        int run_axis = -1;
        int normal_axis = -1;
        int normal_sign = 0;
        int fixed_coord = 0;
        int lo = 0;
        int hi = 0;
        long long prefix = 0;

        long long length() const {
            return (long long)hi - lo;
        }
    };

    struct Cycle {
        vector<Segment> segs;
        long long len = 0;
    };

    int inv_axis = -1;
    int slice_coord = 0;
    int p0 = -1;
    int p1 = -1;
    vector<int> coord0;
    vector<int> coord1;
    vector<vector<char>> occ;
    vector<PrimitiveEdge> edges;
    vector<vector<array<int, 4>>> id_at;
    vector<int> next_edge;
    vector<Cycle> cycles;

    static int dir_axis(int dir, int p0, int p1) {
        return (dir % 2 == 0) ? p0 : p1;
    }

    static int dir_sign(int dir) {
        return (dir < 2) ? 1 : -1;
    }

    pair<int, int> edge_end(int sx, int sy, int dir) const {
        if (dir == 0) {
            return {sx + 1, sy};
        }
        if (dir == 1) {
            return {sx, sy + 1};
        }
        if (dir == 2) {
            return {sx - 1, sy};
        }
        return {sx, sy - 1};
    }

    Segment make_segment(const PrimitiveEdge& e) const {
        Segment seg;
        seg.dir = e.dir;
        seg.run_axis = dir_axis(e.dir, p0, p1);
        seg.normal_axis = e.normal_axis;
        seg.normal_sign = e.normal_sign;
        if (e.dir == 0) {
            seg.fixed_coord = coord1[e.sy];
            seg.lo = coord0[e.sx];
            seg.hi = coord0[e.sx + 1];
        } else if (e.dir == 1) {
            seg.fixed_coord = coord0[e.sx];
            seg.lo = coord1[e.sy];
            seg.hi = coord1[e.sy + 1];
        } else if (e.dir == 2) {
            seg.fixed_coord = coord1[e.sy];
            seg.lo = coord0[e.sx - 1];
            seg.hi = coord0[e.sx];
        } else {
            seg.fixed_coord = coord0[e.sx];
            seg.lo = coord1[e.sy - 1];
            seg.hi = coord1[e.sy];
        }
        return seg;
    }

    int next_id(int id) const {
        const PrimitiveEdge& e = edges[id];
        pair<int, int> endp = edge_end(e.sx, e.sy, e.dir);
        int ex = endp.first;
        int ey = endp.second;
        const int order[4] = {(e.dir + 1) & 3, e.dir, (e.dir + 3) & 3, (e.dir + 2) & 3};
        for (int nd : order) {
            int cand = id_at[ex][ey][nd];
            if (cand != -1) {
                return cand;
            }
        }
        return -1;
    }
};

struct Robot {
    int inv_axis = -1;
    int slice_coord = 0;
    int cycle_id = -1;
    long long period = 0;
    long long start_index = 0;
    int dir_sign = 0;
    const Slice* slice = nullptr;
};

Slice build_slice(const vector<Box>& boxes, int inv_axis, int slice_coord) {
    Slice sl;
    sl.inv_axis = inv_axis;
    sl.slice_coord = slice_coord;
    vector<int> plane_axes;
    for (int axis = 0; axis < 3; ++axis) {
        if (axis != inv_axis) {
            plane_axes.push_back(axis);
        }
    }
    sl.p0 = plane_axes[0];
    sl.p1 = plane_axes[1];

    struct Rect {
        int u1, u2, v1, v2;
    };
    vector<Rect> rects;
    for (const Box& box : boxes) {
        if (box.lo[inv_axis] <= slice_coord && slice_coord < box.hi[inv_axis]) {
            rects.push_back({box.lo[sl.p0], box.hi[sl.p0], box.lo[sl.p1], box.hi[sl.p1]});
            sl.coord0.push_back(box.lo[sl.p0]);
            sl.coord0.push_back(box.hi[sl.p0]);
            sl.coord1.push_back(box.lo[sl.p1]);
            sl.coord1.push_back(box.hi[sl.p1]);
        }
    }

    sort(sl.coord0.begin(), sl.coord0.end());
    sl.coord0.erase(unique(sl.coord0.begin(), sl.coord0.end()), sl.coord0.end());
    sort(sl.coord1.begin(), sl.coord1.end());
    sl.coord1.erase(unique(sl.coord1.begin(), sl.coord1.end()), sl.coord1.end());

    const int w = (int)sl.coord0.size();
    const int h = (int)sl.coord1.size();
    sl.occ.assign(max(0, w - 1), vector<char>(max(0, h - 1), 0));

    for (const Rect& rect : rects) {
        int i1 = (int)(lower_bound(sl.coord0.begin(), sl.coord0.end(), rect.u1) - sl.coord0.begin());
        int i2 = (int)(lower_bound(sl.coord0.begin(), sl.coord0.end(), rect.u2) - sl.coord0.begin());
        int j1 = (int)(lower_bound(sl.coord1.begin(), sl.coord1.end(), rect.v1) - sl.coord1.begin());
        int j2 = (int)(lower_bound(sl.coord1.begin(), sl.coord1.end(), rect.v2) - sl.coord1.begin());
        for (int i = i1; i < i2; ++i) {
            for (int j = j1; j < j2; ++j) {
                sl.occ[i][j] = 1;
            }
        }
    }

    sl.id_at.assign(w, vector<array<int, 4>>(h));
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            sl.id_at[i][j].fill(-1);
        }
    }

    auto add_edge = [&](int sx, int sy, int dir, long long len, int normal_axis, int normal_sign) {
        Slice::PrimitiveEdge e;
        e.sx = sx;
        e.sy = sy;
        e.dir = dir;
        e.len = len;
        e.normal_axis = normal_axis;
        e.normal_sign = normal_sign;
        int id = (int)sl.edges.size();
        sl.edges.push_back(e);
        sl.id_at[sx][sy][dir] = id;
    };

    for (int i = 0; i + 1 < w; ++i) {
        for (int j = 0; j + 1 < h; ++j) {
            if (!sl.occ[i][j]) {
                continue;
            }
            if (j == 0 || !sl.occ[i][j - 1]) {
                add_edge(i, j, 0, (long long)sl.coord0[i + 1] - sl.coord0[i], sl.p1, -1);
            }
            if (i + 1 == w - 1 || !sl.occ[i + 1][j]) {
                add_edge(i + 1, j, 1, (long long)sl.coord1[j + 1] - sl.coord1[j], sl.p0, +1);
            }
            if (j + 1 == h - 1 || !sl.occ[i][j + 1]) {
                add_edge(i + 1, j + 1, 2, (long long)sl.coord0[i + 1] - sl.coord0[i], sl.p1, +1);
            }
            if (i == 0 || !sl.occ[i - 1][j]) {
                add_edge(i, j + 1, 3, (long long)sl.coord1[j + 1] - sl.coord1[j], sl.p0, -1);
            }
        }
    }

    sl.next_edge.assign(sl.edges.size(), -1);
    for (int id = 0; id < (int)sl.edges.size(); ++id) {
        sl.next_edge[id] = sl.next_id(id);
    }

    vector<char> vis(sl.edges.size(), 0);
    for (int start = 0; start < (int)sl.edges.size(); ++start) {
        if (vis[start]) {
            continue;
        }
        vector<int> path;
        int cur = start;
        while (!vis[cur]) {
            vis[cur] = 1;
            path.push_back(cur);
            cur = sl.next_edge[cur];
        }

        Slice::Cycle cyc;
        for (int id : path) {
            const auto& e = sl.edges[id];
            Slice::Segment seg = sl.make_segment(e);
            if (!cyc.segs.empty()) {
                Slice::Segment& last = cyc.segs.back();
                if (last.dir == seg.dir && last.normal_axis == seg.normal_axis &&
                    last.normal_sign == seg.normal_sign && last.fixed_coord == seg.fixed_coord) {
                    last.lo = min(last.lo, seg.lo);
                    last.hi = max(last.hi, seg.hi);
                    continue;
                }
            }
            cyc.segs.push_back(seg);
        }

        long long pref = 0;
        for (auto& seg : cyc.segs) {
            seg.prefix = pref;
            pref += seg.length();
        }
        cyc.len = pref;
        sl.cycles.push_back(move(cyc));
    }

    return sl;
}

long long unit_offset_on_segment(const Slice::Segment& seg, int lower_coord) {
    if (seg.dir == 0 || seg.dir == 1) {
        return (long long)lower_coord - seg.lo;
    }
    return (long long)seg.hi - 1 - lower_coord;
}

long long residue_to_face(const Robot& robot, long long face_index) {
    if (robot.dir_sign == 1) {
        return mod_norm(face_index - robot.start_index, robot.period);
    }
    return mod_norm(robot.start_index - face_index, robot.period);
}

Robot map_robot(const RobotInput& in, const Slice& sl) {
    Robot out;
    out.inv_axis = sl.inv_axis;
    out.slice_coord = sl.slice_coord;
    out.slice = &sl;

    int plane_dir = -1;
    if (in.move.axis == sl.p0) {
        plane_dir = (in.move.sign == 1 ? 0 : 2);
    } else {
        plane_dir = (in.move.sign == 1 ? 1 : 3);
    }

    int fixed_coord = in.cube[in.face.axis] + (in.face.sign == 1 ? 1 : 0);
    int lower_coord = in.cube[in.move.axis];

    for (int cid = 0; cid < (int)sl.cycles.size(); ++cid) {
        const auto& cyc = sl.cycles[cid];
        for (const auto& seg : cyc.segs) {
            if (seg.normal_axis != in.face.axis || seg.normal_sign != in.face.sign) {
                continue;
            }
            if (seg.run_axis != in.move.axis || seg.fixed_coord != fixed_coord) {
                continue;
            }
            if (lower_coord < seg.lo || lower_coord >= seg.hi) {
                continue;
            }
            out.cycle_id = cid;
            out.period = cyc.len;
            out.start_index = seg.prefix + unit_offset_on_segment(seg, lower_coord);
            out.dir_sign = (seg.dir == plane_dir ? 1 : -1);
            return out;
        }
    }

    throw runtime_error("Failed to map robot to slice cycle");
}

long long earliest_same_cycle_face(const Robot& a, const Robot& b) {
    long long len = a.period;
    if (a.dir_sign == b.dir_sign) {
        return (a.start_index == b.start_index) ? 0 : INF64;
    }
    long long t = 0;
    return solve_linear_congruence(a.dir_sign - b.dir_sign, b.start_index - a.start_index, len, t)
               ? t
               : INF64;
}

long long earliest_same_cycle_swap(const Robot& a, const Robot& b) {
    if (a.dir_sign + b.dir_sign != 0) {
        return INF64;
    }
    long long len = a.period;
    long long t = 0;
    return solve_linear_congruence(b.dir_sign - a.dir_sign,
                                   a.dir_sign - (b.start_index - a.start_index), len, t)
               ? t
               : INF64;
}

long long earliest_cross_axis_face(const Robot& a, const Robot& b) {
    int other = 3 - a.inv_axis - b.inv_axis;
    const auto& cyc_a = a.slice->cycles[a.cycle_id];
    unordered_map<long long, long long> residues;
    residues.reserve(cyc_a.segs.size() * 2 + 1);

    for (const auto& seg : cyc_a.segs) {
        if (seg.normal_axis != other || seg.run_axis != b.inv_axis) {
            continue;
        }
        if (b.slice_coord < seg.lo || b.slice_coord >= seg.hi) {
            continue;
        }
        long long key = ((long long)seg.fixed_coord << 1) | (seg.normal_sign == 1 ? 1LL : 0LL);
        long long idx = seg.prefix + unit_offset_on_segment(seg, b.slice_coord);
        residues.emplace(key, residue_to_face(a, idx));
    }

    long long best = INF64;
    const auto& cyc_b = b.slice->cycles[b.cycle_id];
    for (const auto& seg : cyc_b.segs) {
        if (seg.normal_axis != other || seg.run_axis != a.inv_axis) {
            continue;
        }
        if (a.slice_coord < seg.lo || a.slice_coord >= seg.hi) {
            continue;
        }
        long long key = ((long long)seg.fixed_coord << 1) | (seg.normal_sign == 1 ? 1LL : 0LL);
        auto it = residues.find(key);
        if (it == residues.end()) {
            continue;
        }
        long long idx = seg.prefix + unit_offset_on_segment(seg, a.slice_coord);
        long long rb = residue_to_face(b, idx);
        long long t = 0;
        if (crt_two(it->second, a.period, rb, b.period, t)) {
            best = min(best, t);
        }
    }
    return best;
}

void solve() {
    int n = 0, k = 0;
    cin >> n >> k;
    vector<Box> boxes(n);
    for (int i = 0; i < n; ++i) {
        for (int axis = 0; axis < 3; ++axis) {
            cin >> boxes[i].lo[axis];
        }
        for (int axis = 0; axis < 3; ++axis) {
            cin >> boxes[i].hi[axis];
        }
    }

    vector<RobotInput> input(k);
    for (int i = 0; i < k; ++i) {
        string fs, ds;
        cin >> input[i].cube[0] >> input[i].cube[1] >> input[i].cube[2] >> fs >> ds;
        input[i].face = parse_axis_dir(fs);
        input[i].move = parse_axis_dir(ds);
    }

    if (k < 2) {
        cout << "ok\n";
        return;
    }

    map<pair<int, int>, int> slice_index;
    vector<Slice> slices;
    for (const auto& r : input) {
        int inv_axis = 3 - r.face.axis - r.move.axis;
        int slice_coord = r.cube[inv_axis];
        pair<int, int> key = {inv_axis, slice_coord};
        if (!slice_index.count(key)) {
            int id = (int)slices.size();
            slice_index[key] = id;
            slices.push_back(build_slice(boxes, inv_axis, slice_coord));
        }
    }

    vector<Robot> robots;
    robots.reserve(k);
    for (const auto& r : input) {
        int inv_axis = 3 - r.face.axis - r.move.axis;
        int slice_coord = r.cube[inv_axis];
        const Slice& sl = slices[slice_index[{inv_axis, slice_coord}]];
        robots.push_back(map_robot(r, sl));
    }

    long long best = INF64;
    for (int i = 0; i < k; ++i) {
        for (int j = i + 1; j < k; ++j) {
            const Robot& a = robots[i];
            const Robot& b = robots[j];
            long long cand = INF64;
            if (a.inv_axis == b.inv_axis) {
                if (a.slice_coord == b.slice_coord && a.slice == b.slice && a.cycle_id == b.cycle_id) {
                    cand = min(cand, earliest_same_cycle_face(a, b));
                    cand = min(cand, earliest_same_cycle_swap(a, b));
                }
            } else {
                cand = min(cand, earliest_cross_axis_face(a, b));
            }
            best = min(best, cand);
        }
    }

    if (best == INF64) {
        cout << "ok\n";
    } else {
        cout << best << '\n';
    }
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
