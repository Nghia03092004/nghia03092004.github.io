#include <bits/stdc++.h>
using namespace std;

// Grader-provided function.
int getDistance(int i, int j);

namespace {

int anchor_v = 0;
int anchor_s = 0;
vector<int> dist_from_v;
vector<int> dist_from_s;
unordered_map<unsigned long long, int> cached_queries;

unsigned long long make_key(int a, int b) {
    if (a > b) swap(a, b);
    return (static_cast<unsigned long long>(a) << 32) ^
           static_cast<unsigned int>(b);
}

int query_distance(int a, int b) {
    if (a == b) return 0;
    if (a == anchor_v) return dist_from_v[b];
    if (b == anchor_v) return dist_from_v[a];
    if (a == anchor_s) return dist_from_s[b];
    if (b == anchor_s) return dist_from_s[a];

    unsigned long long key = make_key(a, b);
    auto it = cached_queries.find(key);
    if (it != cached_queries.end()) return it->second;

    int value = getDistance(a, b);
    cached_queries[key] = value;
    return value;
}

bool same_component(int x, int y, long long pivot) {
    long long shared =
        static_cast<long long>(dist_from_s[x]) + dist_from_s[y] - query_distance(x, y);
    return shared > 2 * pivot;
}

bool has_giant_component(const vector<int> &group, long long pivot, int total_leaves) {
    struct Block {
        int rep;
        int size;
    };

    vector<Block> alive;
    alive.reserve(group.size());
    for (int leaf : group) alive.push_back({leaf, 1});

    vector<Block> dead;
    dead.reserve(group.size());

    while (alive.size() > 1) {
        vector<Block> next;
        next.reserve(alive.size() / 2);

        for (int i = 0; i + 1 < (int)alive.size(); i += 2) {
            const Block &a = alive[i];
            const Block &b = alive[i + 1];
            if (same_component(a.rep, b.rep, pivot)) {
                next.push_back({a.rep, a.size + b.size});
            } else {
                dead.push_back(a);
                dead.push_back(b);
            }
        }
        if (alive.size() % 2 == 1) dead.push_back(alive.back());
        alive.swap(next);
    }

    if (alive.empty()) return false;

    int majority_size = alive[0].size;
    int candidate = alive[0].rep;
    for (const Block &block : dead) {
        if (same_component(candidate, block.rep, pivot)) {
            majority_size += block.size;
        }
    }
    return majority_size > total_leaves / 2;
}

}  // namespace

int hubDistance(int N, int sub) {
    anchor_v = 0;
    dist_from_v.assign(N, 0);
    cached_queries.clear();

    for (int i = 1; i < N; ++i) dist_from_v[i] = getDistance(anchor_v, i);

    anchor_s = 0;
    for (int i = 1; i < N; ++i) {
        if (dist_from_v[i] > dist_from_v[anchor_s]) anchor_s = i;
    }

    dist_from_s.assign(N, 0);
    for (int i = 0; i < N; ++i) {
        if (i == anchor_s) continue;
        dist_from_s[i] = (i == anchor_v ? dist_from_v[anchor_s] : getDistance(anchor_s, i));
    }

    int diameter_endpoint = 0;
    for (int i = 1; i < N; ++i) {
        if (dist_from_s[i] > dist_from_s[diameter_endpoint]) diameter_endpoint = i;
    }
    long long diameter = dist_from_s[diameter_endpoint];
    long long sv_length = dist_from_v[anchor_s];

    vector<long long> projection(N);
    vector<long long> positions;
    positions.reserve(N);
    for (int i = 0; i < N; ++i) {
        projection[i] =
            (static_cast<long long>(dist_from_s[i]) + sv_length - dist_from_v[i]) / 2;
        positions.push_back(projection[i]);
    }

    sort(positions.begin(), positions.end());
    positions.erase(unique(positions.begin(), positions.end()), positions.end());

    long long radius = LLONG_MAX;
    vector<long long> centers;
    for (long long pos : positions) {
        if (pos == 0 || pos == sv_length) continue;
        long long eccentricity = max(pos, diameter - pos);
        if (eccentricity < radius) {
            radius = eccentricity;
            centers.assign(1, pos);
        } else if (eccentricity == radius) {
            centers.push_back(pos);
        }
    }

    if (radius == LLONG_MAX) return 0;
    if (sub <= 2) return static_cast<int>(radius);

    vector<long long> sorted_projection = projection;
    sort(sorted_projection.begin(), sorted_projection.end());
    long long median_left = sorted_projection[(N - 1) / 2];
    long long median_right = sorted_projection[N / 2];

    bool balanced = false;
    for (long long center : centers) {
        if (center < median_left || center > median_right) continue;

        if (median_left != median_right) {
            balanced = true;
            break;
        }

        vector<int> attached_leaves;
        attached_leaves.reserve(N);
        for (int i = 0; i < N; ++i) {
            if (projection[i] == center) attached_leaves.push_back(i);
        }

        if ((int)attached_leaves.size() <= N / 2 ||
            !has_giant_component(attached_leaves, center, N)) {
            balanced = true;
            break;
        }
    }

    return balanced ? static_cast<int>(radius) : -static_cast<int>(radius);
}

int main() {
    // Grader handles interaction.
    return 0;
}
