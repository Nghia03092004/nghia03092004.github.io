#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 292: Pythagorean Polygons
 *
 * Count convex polygons with lattice-point vertices, integer edge lengths,
 * at least 3 vertices, no 3 collinear, perimeter <= 120.
 * Distinct up to translation.
 *
 * Approach: Enumerate edge directions sorted by angle. DFS picks directions
 * in strictly increasing angular order with multipliers for edge lengths.
 * Polygon closes when displacement returns to origin with >= 3 edges.
 * Memoize on (dx, dy, last_direction_index, remaining_perimeter).
 */

const int L = 120;

struct Direction {
    int pa, pb;
    int base_len;
};

vector<Direction> dirs;
int ndirs;

void generate_directions() {
    set<pair<int,int>> seen;
    for (int d = 1; d <= L; d++) {
        for (int a = -d; a <= d; a++) {
            long long b2 = (long long)d*d - (long long)a*a;
            int b = (int)round(sqrt((double)b2));
            if ((long long)b*b != b2) continue;
            for (int sb : {1, -1}) {
                int bb = b * sb;
                if (b == 0 && sb == -1) continue;
                int g = __gcd(abs(a), abs(bb));
                if (g == 0) continue;
                int pa = a/g, pb = bb/g;
                if (seen.count({pa,pb})) continue;
                int base2 = pa*pa + pb*pb;
                int base = (int)round(sqrt((double)base2));
                if (base*base != base2) continue;
                seen.insert({pa,pb});
                dirs.push_back({pa, pb, base});
            }
        }
    }
    sort(dirs.begin(), dirs.end(), [](const Direction& a, const Direction& b) {
        double aa = atan2(a.pb, a.pa);
        double ab = atan2(b.pb, b.pa);
        return aa < ab;
    });
    ndirs = dirs.size();
}

// Memoization using hash map
// State: (dx, dy, last_dir_index, remaining_perimeter)
// Value: number of ways to close the polygon from this state (with >= min_edges more)

// For the DP, we want:
// f(dx, dy, i, rem) = number of closed convex polygons reachable by
//   picking directions j > i with multipliers, such that sum of new edges
//   brings (dx,dy) back to (0,0), total new edge lengths <= rem,
//   and we pick at least 'needed' more edges.
// But 'needed' depends on how many edges we've already picked (need total >= 3).
// We handle this by calling with needed = max(0, 3 - edges_so_far).

// Actually, let's restructure. We count using:
// count(i, dx, dy, rem) = number of ways to pick edges from directions i+1, i+2, ..., ndirs-1
//   such that the sum of edge vectors = (-dx, -dy) and total length <= rem,
//   returning the count. The edges picked must be at least 'edges_needed'.
// We call this with edges_needed embedded in the state via a flag.

// Simpler: split into two cases.
// Case 1: we've already picked >= 3 edges. Then any closure (including 0 more edges) counts.
// Case 2: we've picked < 3 edges. Need more.

// For memoization, the state (dx, dy, i, rem) is enough if we also track
// how many edges we've picked. But edges_picked can be 0..~120, too many.
// Instead, track min(edges_picked, 3) which is 0, 1, 2, or 3+.

// State: (dx, dy, last_dir_index, remaining_perim, min(edges_so_far, 3))
// dx, dy in range [-120, 120], i in [0, ndirs], rem in [0, 120], ne in {0,1,2,3}

// This is feasible with a hash map or array.

// For L=120, dx and dy range roughly [-120, 120], but with Pythagorean
// edges the actual range might be larger (since pa, pb can be up to 120).
// Actually pa, pb are primitive directions with pa^2+pb^2 = base^2.
// With max multiplier k = L / base, max displacement per edge = k * max(|pa|,|pb|).
// But cumulative displacement is bounded by remaining perimeter (distance <= rem).
// So |dx|, |dy| <= L = 120 (since distance <= perimeter).
// Actually, max displacement: if all edges go in similar directions, dx could
// be up to L. But sum of |edge vectors| = sum of edge lengths = perimeter <= L.
// And by triangle inequality, displacement <= sum of edge lengths = perimeter.
// So sqrt(dx^2+dy^2) <= L, meaning |dx|, |dy| <= L.

// Hash map for memoization
unordered_map<long long, long long> memo;

long long encode(int dx, int dy, int i, int rem, int ne) {
    // dx in [-120,120] -> [0, 240], dy same, i in [0,200], rem in [0,120], ne in [0,3]
    long long x = (long long)(dx + 120);
    long long y = (long long)(dy + 120);
    return ((((x * 241 + y) * 200 + i) * 121 + rem) * 4 + ne);
}

long long dfs(int dx, int dy, int last_idx, int rem, int ne);

long long dfs_memo(int dx, int dy, int last_idx, int rem, int ne) {
    int ne_key = min(ne, 3);
    long long key = encode(dx, dy, last_idx + 1, rem, ne_key);
    auto it = memo.find(key);
    if (it != memo.end()) return it->second;
    long long result = dfs(dx, dy, last_idx, rem, ne);
    memo[key] = result;
    return result;
}

long long dfs(int dx, int dy, int last_idx, int rem, int ne) {
    long long count = 0;
    for (int i = last_idx + 1; i < ndirs; i++) {
        int pa = dirs[i].pa, pb = dirs[i].pb;
        int base = dirs[i].base_len;
        if (base > rem) continue;
        int max_k = rem / base;
        for (int k = 1; k <= max_k; k++) {
            int ndx = dx + k * pa, ndy = dy + k * pb;
            int nrem = rem - k * base;
            int nne = ne + 1;
            if (ndx == 0 && ndy == 0 && nne >= 3) {
                count++;
            }
            // Pruning
            double dist = sqrt((double)ndx*ndx + (double)ndy*ndy);
            if (dist > nrem + 0.001) continue;
            if (nrem > 0 && i + 1 < ndirs) {
                count += dfs_memo(ndx, ndy, i, nrem, nne);
            }
        }
    }
    return count;
}

int main() {
    generate_directions();

    memo.clear();
    memo.reserve(1 << 22);
    long long ans = dfs_memo(0, 0, -1, L, 0);
    cout << ans << endl;
    return 0;
}
