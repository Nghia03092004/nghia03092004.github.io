#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const ll MOD = 100000007LL;

// Transfer matrix (sparse representation)
vector<vector<pair<int,int>>> T(512); // T[i] = list of (j, count)

void fill(int pos, int cur_mask, int next_mask, int start_mask, vector<vector<pair<int,int>>>& T_build) {
    while (pos < 9 && ((cur_mask >> pos) & 1)) pos++;
    if (pos == 9) {
        // Add transition start_mask -> next_mask
        for (auto& p : T_build[start_mask]) {
            if (p.first == next_mask) { p.second++; return; }
        }
        T_build[start_mask].push_back({next_mask, 1});
        return;
    }
    int r = pos / 3, c = pos % 3;
    if (c + 1 < 3 && !((cur_mask >> (pos+1)) & 1))
        fill(pos+1, cur_mask | (1<<pos) | (1<<(pos+1)), next_mask, start_mask, T_build);
    if (r + 1 < 3 && !((cur_mask >> (pos+3)) & 1))
        fill(pos+1, cur_mask | (1<<pos) | (1<<(pos+3)), next_mask, start_mask, T_build);
    fill(pos+1, cur_mask | (1<<pos), next_mask | (1<<pos), start_mask, T_build);
}

void build_transfer() {
    for (int s = 0; s < 512; s++) {
        fill(0, s, 0, s, T);
    }
}

// Berlekamp-Massey
pair<int, vector<ll>> berlekamp_massey(const vector<ll>& s) {
    int n = s.size();
    vector<ll> C = {1}, B = {1};
    int L = 0, m = 1;
    ll b = 1;

    auto modinv = [](ll a, ll mod) -> ll {
        ll g = mod, x = 0, y = 1;
        ll aa = a;
        while (aa != 0) {
            ll q = g / aa;
            g -= q * aa; swap(g, aa);
            x -= q * y; swap(x, y);
        }
        return (x % mod + mod) % mod;
    };

    for (int i = 0; i < n; i++) {
        ll d = s[i];
        for (int j = 1; j <= L; j++)
            d = (d + C[j] % MOD * (s[i-j] % MOD)) % MOD;
        d = (d % MOD + MOD) % MOD;

        if (d == 0) { m++; continue; }
        if (2*L <= i) {
            vector<ll> T_save = C;
            ll coef = d % MOD * modinv(b, MOD) % MOD;
            while ((int)C.size() < (int)B.size() + m) C.push_back(0);
            for (int j = 0; j < (int)B.size(); j++)
                C[j+m] = (C[j+m] - coef % MOD * (B[j] % MOD) % MOD + MOD) % MOD;
            L = i + 1 - L;
            B = T_save;
            b = d;
            m = 1;
        } else {
            ll coef = d % MOD * modinv(b, MOD) % MOD;
            while ((int)C.size() < (int)B.size() + m) C.push_back(0);
            for (int j = 0; j < (int)B.size(); j++)
                C[j+m] = (C[j+m] - coef % MOD * (B[j] % MOD) % MOD + MOD) % MOD;
            m++;
        }
    }
    return {L, C};
}

// Polynomial multiplication mod modpoly mod MOD
vector<ll> poly_mul(const vector<ll>& a, const vector<ll>& b, const vector<ll>& modpoly) {
    int d = modpoly.size() - 1;
    vector<ll> c(a.size() + b.size() - 1, 0);
    for (int i = 0; i < (int)a.size(); i++) {
        if (a[i] == 0) continue;
        for (int j = 0; j < (int)b.size(); j++)
            c[i+j] = (c[i+j] + a[i] * b[j]) % MOD;
    }

    auto modinv = [](ll a, ll mod) -> ll {
        ll g = mod, x = 0, y = 1;
        ll aa = a;
        while (aa != 0) {
            ll q = g / aa;
            g -= q * aa; swap(g, aa);
            x -= q * y; swap(x, y);
        }
        return (x % mod + mod) % mod;
    };

    ll lc_inv = modinv(modpoly[d], MOD);
    for (int i = (int)c.size() - 1; i >= d; i--) {
        if (c[i] == 0) continue;
        ll coef = c[i] % MOD * lc_inv % MOD;
        for (int j = 0; j <= d; j++)
            c[i-d+j] = (c[i-d+j] - coef * modpoly[j] % MOD + MOD) % MOD;
    }
    c.resize(d);
    return c;
}

int main() {
    build_transfer();

    // Compute sequence values
    vector<ll> v(512, 0);
    v[0] = 1;
    vector<ll> vals;
    vals.push_back(v[0]);

    for (int step = 0; step < 200; step++) {
        vector<ll> nv(512, 0);
        for (int i = 0; i < 512; i++) {
            if (v[i] == 0) continue;
            for (auto& [j, cnt] : T[i]) {
                nv[j] = (nv[j] + v[i] * cnt) % MOD;
            }
        }
        v = nv;
        vals.push_back(v[0]);
    }

    // Berlekamp-Massey
    auto [L, C] = berlekamp_massey(vals);

    // Build modpoly
    vector<ll> modpoly(L + 1);
    for (int j = 0; j <= L; j++)
        modpoly[j] = (C[L - j] % MOD + MOD) % MOD;

    // Compute 10^10000 in binary
    // We'll do polynomial exponentiation: start with x, square-and-multiply
    // But we need to represent 10^10000 in binary.
    // Strategy: compute by repeated squaring of the exponent itself? No.
    // Better: compute poly^10 repeatedly 10000 times (each time raise to 10th power).

    // poly_pow_10: raise polynomial to 10th power
    // poly^10 = ((poly^2)^2 * poly)^2  (10 = 1010 in binary)

    vector<ll> result(L, 0);
    result[0] = 1; // polynomial 1

    vector<ll> base(L, 0);
    base[1] = 1; // polynomial x

    // Compute base^(10^10000) = (...((base^10)^10)^10...)^10 (10000 times)
    // Start: cur = x
    // Repeat 10000 times: cur = cur^10

    auto poly_pow10 = [&](vector<ll> p) -> vector<ll> {
        // p^10 = p^8 * p^2
        auto p2 = poly_mul(p, p, modpoly);
        auto p4 = poly_mul(p2, p2, modpoly);
        auto p8 = poly_mul(p4, p4, modpoly);
        return poly_mul(p8, p2, modpoly);
    };

    vector<ll> cur = base; // x
    for (int i = 0; i < 10000; i++) {
        cur = poly_pow10(cur);
    }

    // Evaluate: f(n) = sum cur[i] * vals[i]
    ll answer = 0;
    for (int i = 0; i < L; i++) {
        answer = (answer + cur[i] % MOD * (vals[i] % MOD)) % MOD;
    }
    answer = (answer + MOD) % MOD;

    cout << answer << endl;
    return 0;
}
