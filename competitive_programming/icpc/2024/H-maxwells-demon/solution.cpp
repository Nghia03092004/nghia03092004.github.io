#include <bits/stdc++.h>
using namespace std;

namespace {

struct TimeFrac {
    int scaled = 0;
    int denom = 1;
};

bool operator<(const TimeFrac& a, const TimeFrac& b) {
    return 1LL * a.scaled * b.denom < 1LL * b.scaled * a.denom;
}

bool same_time(const TimeFrac& a, const TimeFrac& b) {
    return 1LL * a.scaled * b.denom == 1LL * b.scaled * a.denom;
}

struct XorVector {
    array<unsigned long long, 4> words{};

    bool any() const {
        return words[0] || words[1] || words[2] || words[3];
    }
};

void xor_with(XorVector& a, const XorVector& b) {
    for (int i = 0; i < 4; ++i) {
        a.words[i] ^= b.words[i];
    }
}

int highest_bit(const XorVector& v) {
    for (int block = 3; block >= 0; --block) {
        if (v.words[block] == 0) {
            continue;
        }
        return block * 64 + 63 - __builtin_clzll(v.words[block]);
    }
    return -1;
}

long long extended_gcd(long long a, long long b, long long& x, long long& y) {
    if (b == 0) {
        x = (a >= 0 ? 1 : -1);
        y = 0;
        return llabs(a);
    }
    long long x1 = 0;
    long long y1 = 0;
    const long long g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

long long gcd_ll(long long a, long long b) {
    a = llabs(a);
    b = llabs(b);
    while (b != 0) {
        const long long r = a % b;
        a = b;
        b = r;
    }
    return a;
}

long long mod_inverse(long long a, long long mod) {
    long long x = 0;
    long long y = 0;
    const long long g = extended_gcd(a, mod, x, y);
    if (g != 1) {
        return -1;
    }
    x %= mod;
    if (x < 0) {
        x += mod;
    }
    return x;
}

long long positive_mod(long long value, long long mod) {
    value %= mod;
    if (value < 0) {
        value += mod;
    }
    return value;
}

vector<int> generate_times(const int w,
                           const int h,
                           const int d,
                           const int px,
                           const int py,
                           const int vx,
                           const int vy) {
    const int q0 = abs(px);
    const int qv = (px > 0 ? vx : -vx);
    const int ax = abs(qv);
    if (ax == 0) {
        return {};
    }

    const int s0 = (qv < 0 ? q0 : 2 * w - q0);
    const long long limit_scaled = 1LL * 2 * w * h * ax;
    vector<int> hits;

    vector<int> targets = {d};
    if (2 * h - d != d) {
        targets.push_back(2 * h - d);
    }

    for (const int target_y : targets) {
        const long long A = 1LL * 2 * w * vy;
        const long long M = 1LL * 2 * h * ax;
        const long long B =
            1LL * ax * (target_y - py) - 1LL * vy * s0;

        long long first_k = -1;
        long long step_k = -1;

        if (A == 0) {
            if (positive_mod(B, M) == 0) {
                first_k = 0;
                step_k = 1;
            }
        } else {
            const long long g = gcd_ll(A, M);
            if (B % g == 0) {
                const long long A2 = A / g;
                const long long B2 = B / g;
                const long long M2 = M / g;
                const long long inv = mod_inverse(positive_mod(A2, M2), M2);
                first_k = positive_mod(inv * positive_mod(B2, M2), M2);
                step_k = M2;
            }
        }

        if (first_k == -1) {
            continue;
        }

        for (long long k = first_k;; k += step_k) {
            const long long scaled = 1LL * s0 + 1LL * 2 * w * k;
            if (scaled >= limit_scaled) {
                break;
            }
            hits.push_back(static_cast<int>(scaled));
            if (step_k == 0) {
                break;
            }
        }
    }

    sort(hits.begin(), hits.end());
    hits.erase(unique(hits.begin(), hits.end()), hits.end());
    return hits;
}

bool insert_vector(XorVector value, array<XorVector, 256>& basis) {
    while (value.any()) {
        const int bit = highest_bit(value);
        if (!basis[bit].any()) {
            basis[bit] = value;
            return true;
        }
        xor_with(value, basis[bit]);
    }
    return false;
}

bool in_span(XorVector value, const array<XorVector, 256>& basis) {
    while (value.any()) {
        const int bit = highest_bit(value);
        if (!basis[bit].any()) {
            return false;
        }
        xor_with(value, basis[bit]);
    }
    return true;
}

void solve() {
    int w;
    int h;
    int d;
    int red_count;
    int blue_count;
    cin >> w >> h >> d >> red_count >> blue_count;

    const int particle_count = red_count + blue_count;
    vector<vector<int>> hit_times(particle_count);
    vector<int> denominators(particle_count, 1);
    XorVector target;

    for (int i = 0; i < particle_count; ++i) {
        int px;
        int py;
        int vx;
        int vy;
        cin >> px >> py >> vx >> vy;

        const int initial_side = (px > 0 ? 1 : 0);
        const int desired_side = (i < red_count ? 0 : 1);
        if (initial_side != desired_side) {
            target.words[i / 64] |= 1ULL << (i % 64);
        }

        const int qv = (px > 0 ? vx : -vx);
        denominators[i] = abs(qv);
        hit_times[i] = generate_times(w, h, d, px, py, vx, vy);
    }

    if (!target.any()) {
        cout << "0.0\n";
        return;
    }

    struct HeapNode {
        int particle = 0;
        int index = 0;
    };
    auto cmp = [&](const HeapNode& lhs, const HeapNode& rhs) {
        const TimeFrac a{hit_times[lhs.particle][lhs.index],
                         denominators[lhs.particle]};
        const TimeFrac b{hit_times[rhs.particle][rhs.index],
                         denominators[rhs.particle]};
        return b < a;
    };

    priority_queue<HeapNode, vector<HeapNode>, decltype(cmp)> pq(cmp);
    for (int i = 0; i < particle_count; ++i) {
        if (!hit_times[i].empty()) {
            pq.push({i, 0});
        }
    }

    array<XorVector, 256> basis;
    while (!pq.empty()) {
        const HeapNode first = pq.top();
        pq.pop();
        const TimeFrac current{hit_times[first.particle][first.index],
                               denominators[first.particle]};

        XorVector event;
        event.words[first.particle / 64] |= 1ULL << (first.particle % 64);

        if (first.index + 1 < static_cast<int>(hit_times[first.particle].size())) {
            pq.push({first.particle, first.index + 1});
        }

        while (!pq.empty()) {
            const HeapNode next = pq.top();
            const TimeFrac next_time{hit_times[next.particle][next.index],
                                     denominators[next.particle]};
            if (!same_time(current, next_time)) {
                break;
            }
            pq.pop();
            event.words[next.particle / 64] |= 1ULL << (next.particle % 64);
            if (next.index + 1 <
                static_cast<int>(hit_times[next.particle].size())) {
                pq.push({next.particle, next.index + 1});
            }
        }

        if (insert_vector(event, basis) && in_span(target, basis)) {
            cout << fixed << setprecision(10)
                 << static_cast<long double>(current.scaled) /
                        static_cast<long double>(current.denom)
                 << '\n';
            return;
        }
    }

    cout << "impossible\n";
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    solve();
    return 0;
}
