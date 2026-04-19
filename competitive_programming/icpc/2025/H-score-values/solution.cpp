#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

namespace {

const int64 INF = (1LL << 60);
const int64 NEG = -(1LL << 60);

int64 gcd64(int64 a, int64 b) {
    return b == 0 ? a : gcd64(b, a % b);
}

vector<int> count_digits(int64 x) {
    vector<int> cnt(10, 0);
    if (x == 0) {
        cnt[0] = 1;
        return cnt;
    }
    while (x > 0) {
        cnt[x % 10]++;
        x /= 10;
    }
    return cnt;
}

vector<int> to_padded_digits(int64 x, int len) {
    string s = to_string(x);
    if ((int)s.size() < len) {
        s = string(len - (int)s.size(), '0') + s;
    }
    vector<int> digits(len);
    for (int i = 0; i < len; ++i) {
        digits[i] = s[i] - '0';
    }
    return digits;
}

int dp_mod;
int digit_len;
vector<int> low_d, high_d;
int64 digit_weight[10];
static int64 memo[20][1001][2][2][2];
static unsigned char seen[20][1001][2][2][2];

int64 solve_dp(int pos, int rem, int tight_low, int tight_high, int started) {
    if (pos == digit_len) {
        if (rem == 0) {
            return started ? 0LL : digit_weight[0];
        }
        return NEG;
    }

    int64& ans = memo[pos][rem][tight_low][tight_high][started];
    if (seen[pos][rem][tight_low][tight_high][started]) {
        return ans;
    }
    seen[pos][rem][tight_low][tight_high][started] = 1;
    ans = NEG;

    int lo = tight_low ? low_d[pos] : 0;
    int hi = tight_high ? high_d[pos] : 9;
    for (int d = lo; d <= hi; ++d) {
        int next_started = started || (d != 0);
        int64 add = next_started ? digit_weight[d] : 0LL;
        int next_rem = (rem * 10 + d) % dp_mod;
        int64 suffix = solve_dp(
            pos + 1,
            next_rem,
            tight_low && (d == low_d[pos]),
            tight_high && (d == high_d[pos]),
            next_started
        );
        if (suffix != NEG) {
            ans = max(ans, add + suffix);
        }
    }
    return ans;
}

int64 max_weighted_digits(int64 lo, int64 hi, int mod, const vector<int>& w) {
    if (lo > hi) {
        return NEG;
    }
    string s = to_string(hi);
    digit_len = (int)s.size();
    low_d = to_padded_digits(lo, digit_len);
    high_d.resize(digit_len);
    for (int i = 0; i < digit_len; ++i) {
        high_d[i] = s[i] - '0';
    }
    dp_mod = mod;
    for (int i = 0; i < 10; ++i) {
        digit_weight[i] = w[i];
    }
    memset(seen, 0, sizeof(seen));
    return solve_dp(0, 0, 1, 1, 0);
}

}  // namespace

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int64 m;
    int n;
    cin >> m >> n;
    vector<int> p(n);
    for (int i = 0; i < n; ++i) {
        cin >> p[i];
    }

    int64 g = 0;
    for (int x : p) {
        g = gcd64(g, x);
    }

    vector<int> q(n);
    for (int i = 0; i < n; ++i) {
        q[i] = p[i] / g;
    }

    int a = *min_element(q.begin(), q.end());
    vector<int64> dist(a, INF);
    priority_queue<pair<int64, int>, vector<pair<int64, int>>, greater<pair<int64, int>>> pq;
    dist[0] = 0;
    pq.push({0, 0});
    while (!pq.empty()) {
        pair<int64, int> cur = pq.top();
        pq.pop();
        if (cur.first != dist[cur.second]) {
            continue;
        }
        for (int w : q) {
            int nxt = (cur.second + w) % a;
            int64 nd = cur.first + w;
            if (nd < dist[nxt]) {
                dist[nxt] = nd;
                pq.push({nd, nxt});
            }
        }
    }

    int64 conductor = 0;
    for (int64 x : dist) {
        conductor = max(conductor, x);
    }

    int64 max_reduced = m / g;
    vector<int64> answer(9, 0);

    int64 brute_upto = min(max_reduced, conductor - 1);
    if (conductor == 0) {
        brute_upto = -1;
    }
    for (int64 y = 0; y <= brute_upto; ++y) {
        if (y >= dist[(int)(y % a)]) {
            vector<int> cnt = count_digits(y * g);
            for (int d = 0; d <= 8; ++d) {
                int64 cur = (d == 6 ? (int64)cnt[6] + cnt[9] : cnt[d]);
                answer[d] = max(answer[d], cur);
            }
        }
    }

    int64 suffix_lo = conductor * g;
    if (max_reduced >= conductor) {
        for (int d = 0; d <= 8; ++d) {
            vector<int> w(10, 0);
            if (d == 6) {
                w[6] = 1;
                w[9] = 1;
            } else {
                w[d] = 1;
            }
            answer[d] = max(answer[d], max_weighted_digits(suffix_lo, m, (int)g, w));
        }
    }

    vector<int> cntm = count_digits(m);
    for (int d = 0; d <= 8; ++d) {
        int64 cur = (d == 6 ? (int64)cntm[6] + cntm[9] : cntm[d]);
        answer[d] = max(answer[d], cur);
    }

    for (int d = 0; d <= 8; ++d) {
        if (answer[d] > 0) {
            cout << d << ' ' << answer[d] << '\n';
        }
    }
    return 0;
}
