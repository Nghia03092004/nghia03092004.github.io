#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<ll, ll> pll;

vector<int> to_base(ll x, int p) {
    if (x == 0) return {0};
    vector<int> d;
    while (x > 0) { d.push_back(x % p); x /= p; }
    return d;
}

ll count_geq_digits(ll m, ll n, int p) {
    auto nd = to_base(n, p), md = to_base(m, p);
    int L = max(nd.size(), md.size());
    nd.resize(L, 0);
    md.resize(L, 0);
    reverse(nd.begin(), nd.end());
    reverse(md.begin(), md.end());

    vector<ll> dp(2, 0);
    dp[1] = 1;
    for (int pos = 0; pos < L; pos++) {
        vector<ll> ndp(2, 0);
        for (int t = 0; t < 2; t++) {
            if (!dp[t]) continue;
            int upper = t ? md[pos] : (p - 1);
            for (int d = nd[pos]; d <= upper; d++) {
                int nt = t && (d == md[pos]) ? 1 : 0;
                ndp[nt] += dp[t];
            }
        }
        dp = ndp;
    }
    ll res = dp[0] + dp[1];
    bool all_ok = true;
    for (int j = 0; j < L; j++)
        if (md[j] < nd[j]) { all_ok = false; break; }
    if (all_ok) res--;
    return res;
}

ll count_f10(ll m, ll n) {
    auto n2 = to_base(n, 2), n5 = to_base(n, 5);
    int K = (int)round(log10((double)m));

    ll n_high2 = n >> K;
    ll p5k = 1;
    for (int i = 0; i < K; i++) p5k *= 5;
    ll n_high5 = n / p5k;

    map<pll, ll> states;
    states[{0, 0}] = 1;
    ll pow5 = 1, pow2 = 1;

    for (int k = 0; k < K; k++) {
        map<pll, ll> ns;
        int n2k = (k < (int)n2.size()) ? n2[k] : 0;
        int n5k = (k < (int)n5.size()) ? n5[k] : 0;
        for (auto &[st, cnt] : states) {
            ll c2 = st.first, c5 = st.second;
            for (int d = 0; d < 10; d++) {
                if ((int)((c2 + d) % 2) < n2k) continue;
                if ((int)((c5 + d * pow2) % 5) < n5k) continue;
                ll nc2 = (c2 + d * pow5) / 2;
                ll nc5 = (c5 + d * pow2) / 5;
                ns[{nc2, nc5}] += cnt;
            }
        }
        states = ns;
        pow5 *= 5;
        pow2 *= 2;
    }

    ll total = 0;
    for (auto &[st, cnt] : states) {
        ll c2 = st.first, c5 = st.second;
        if (n_high2 > 0 && (c2 & n_high2) != n_high2) continue;
        if (n_high5 > 0) {
            auto nh = to_base(n_high5, 5), ch = to_base(c5, 5);
            bool ok = true;
            for (int j = 0; j < (int)nh.size(); j++) {
                int cj = (j < (int)ch.size()) ? ch[j] : 0;
                if (cj < nh[j]) { ok = false; break; }
            }
            if (!ok) continue;
        }
        total += cnt;
    }
    return total;
}

int main() {
    ll m = 1000000000000000000LL;
    ll n = 999999999990LL;
    ll f2 = count_geq_digits(m, n, 2);
    ll f5 = count_geq_digits(m, n, 5);
    ll f10 = count_f10(m, n);
    cout << (m - n) - f2 - f5 + f10 << endl;
    return 0;
}
