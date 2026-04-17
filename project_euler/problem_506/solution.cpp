#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef __int128 lll;

const ll MOD = 123454321;
int pat[6] = {1, 2, 3, 4, 3, 2}; // period of digit sequence, sum = 15

ll mod(ll x) { return ((x % MOD) + MOD) % MOD; }
ll power(ll base, ll exp, ll m) {
    ll r = 1; base %= m;
    while (exp > 0) { if (exp & 1) r = r * base % m; base = base * base % m; exp >>= 1; }
    return r;
}

// Geometric sum: 1 + b + b^2 + ... + b^(n-1) mod m
// Using divide and conquer
ll geosum(ll b, ll n, ll m) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n % 2 == 0) {
        // S(n) = S(n/2) * (1 + b^(n/2))
        ll half = geosum(b, n / 2, m);
        ll bhalf = power(b, n / 2, m);
        return half % m * ((1 + bhalf) % m) % m;
    } else {
        // S(n) = 1 + b * S(n-1)
        return (1 + b % m * geosum(b, n - 1, m)) % m;
    }
}

int main() {
    // Verify direct simulation first
    // For v_n starting at position p, greedy: consume digits until digit sum = n
    // The digit sequence repeats with period 6 and sum 15 per cycle.

    // Direct simulation for verification
    {
        int pos = 0;
        ll S_full = 0;
        ll S_mod = 0;
        for (ll n = 1; n <= 1000; n++) {
            ll val = 0;
            ll remaining = n;
            int cnt = 0;
            while (remaining > 0) {
                int d = pat[pos];
                val = val * 10 + d;
                remaining -= d;
                pos = (pos + 1) % 6;
                cnt++;
                if (cnt > 1000) { printf("STUCK\n"); return 1; }
            }
            if (remaining < 0) { printf("OVERSHOT at n=%lld\n", n); return 1; }
            if (n <= 11) S_full += val;
            S_mod = mod(S_mod + mod(val));
            if (n == 11) printf("S(11) = %lld (expected 36120)\n", S_full);
        }
        printf("S(1000) mod %lld = %lld (expected 18232686)\n", MOD, S_mod);
    }

    // OK, the issue with S(1000) is that val can be huge (many digits).
    // val * 10 overflows ll. We need to compute val mod MOD during simulation.
    // But then we can't verify S(11)=36120 exactly. Let me fix.

    {
        int pos = 0;
        ll S_mod = 0;
        for (ll n = 1; n <= 1000; n++) {
            ll val_mod = 0;
            ll remaining = n;
            while (remaining > 0) {
                int d = pat[pos];
                val_mod = (val_mod * 10 + d) % MOD;
                remaining -= d;
                pos = (pos + 1) % 6;
            }
            S_mod = (S_mod + val_mod) % MOD;
        }
        printf("S(1000) mod %lld = %lld (expected 18232686)\n", MOD, S_mod);
    }

    // Now implement efficient algorithm for S(10^14).
    //
    // For v_n starting at position p with digit sum = n:
    //   q = n / 15 (full cycles), r = n % 15 (partial remainder)
    //   Number of digits = 6*q + partial_digits(p, r)
    //   Value mod MOD = cycle_val(p) * 10^(partial_digits(p,r)) * geosum(10^6, q, MOD) + partial_val(p, r)
    //   Ending position = partial_end(p, r) [since full cycles return to p]
    //
    // Precompute for each (p, r):
    //   partial_digits[p][r], partial_val[p][r], partial_end[p][r]
    //   Also cycle_val[p] = 6-digit number from one full cycle starting at p

    ll cycle_val[6];
    for (int p = 0; p < 6; p++) {
        ll v = 0;
        for (int i = 0; i < 6; i++)
            v = (v * 10 + pat[(p + i) % 6]) % MOD;
        cycle_val[p] = v;
    }

    int part_dig[6][15];
    ll part_val[6][15];
    int part_end[6][15];

    for (int p = 0; p < 6; p++) {
        part_dig[p][0] = 0;
        part_val[p][0] = 0;
        part_end[p][0] = p;
        int pp = p;
        ll vv = 0;
        int nd = 0;
        int cumsum = 0;
        // Consume digits greedily, recording state at each achievable partial sum
        bool recorded[15] = {};
        recorded[0] = true;
        while (cumsum < 14) {
            int d = pat[pp];
            vv = (vv * 10 + d) % MOD;
            cumsum += d;
            nd++;
            pp = (pp + 1) % 6;
            if (cumsum <= 14 && !recorded[cumsum]) {
                part_dig[p][cumsum] = nd;
                part_val[p][cumsum] = vv;
                part_end[p][cumsum] = pp;
                recorded[cumsum] = true;
            }
        }
        // Check which remainders are actually reachable
        // If some r is not reachable from position p, it means that combination
        // never occurs in practice.
    }

    // Find the position period.
    // Simulate positions for many n values, tracking position at start of each block.
    {
        int pos = 0;
        vector<int> block_pos;
        block_pos.push_back(pos);
        for (int k = 0; k < 60; k++) {
            for (int j = 1; j <= 15; j++) {
                int r = (15 * k + j) % 15; // r = j % 15
                pos = part_end[pos][r];
            }
            block_pos.push_back(pos);
        }
        int per = 0;
        for (int p = 1; p <= 60; p++) {
            if (block_pos[p] == block_pos[0]) {
                bool ok = true;
                for (int i = 0; i + p <= 60; i++)
                    if (block_pos[i] != block_pos[i + p]) { ok = false; break; }
                if (ok) { per = p; break; }
            }
        }
        printf("Block period = %d (n-period = %d)\n", per, per * 15);
    }

    // Now implement the full solution.
    // Track: for each n, the starting position p and r = n%15.
    // Compute val_n mod MOD = cycle_val[p] * pow10(part_dig[p][r]) * geosum(10^6, n/15, MOD) + part_val[p][r]
    //
    // The position sequence p_n has period T (in n), found above.
    // Within each period, the pattern of (p, r) repeats, but n/15 grows.
    //
    // Strategy: process in periods. In period j, n ranges from j*T+1 to (j+1)*T.
    // For each slot i in [0, T-1], the n value is j*T + i + 1.
    // q = (j*T + i + 1) / 15
    // r = (j*T + i + 1) % 15
    // The starting position for slot i is the same in every period j.
    // So the contribution of slot i in period j is:
    //   cycle_val[p_i] * pow10(L_i) * geosum(10^6, q_ij, MOD) + V_i
    // where q_ij = (j*T + i + 1) / 15, and p_i, L_i, V_i are fixed for slot i.
    //
    // This is still complex because q_ij depends on j in a way tied to integer division.
    // But since T is a multiple of 15 (T = per * 15), we have:
    //   q_ij = (j * T + i + 1) / 15 = j * (T/15) + (i+1)/15
    // So q_ij = j * T15 + q_i0, where T15 = T/15 and q_i0 = (i+1)/15.
    // And r_i = (i+1) % 15 is fixed.
    //
    // geosum(10^6, j*T15 + q_i0) = geosum(10^6, q_i0) + 10^(6*q_i0) * geosum(10^6, j*T15)
    // Actually: geosum(b, m+n) = geosum(b, m) + b^m * geosum(b, n)
    // So geosum(10^6, j*T15 + q_i0) = geosum(10^6, q_i0) + 10^(6*q_i0) * geosum(10^6, j*T15)
    //
    // The contribution of slot i in period j:
    //   C_i * P_i * [geosum(10^6, q_i0) + 10^(6*q_i0) * geosum(10^6, j*T15)] + V_i
    // = C_i * P_i * geosum(10^6, q_i0) + V_i + C_i * P_i * 10^(6*q_i0) * geosum(10^6, j*T15)
    // = (fixed_i) + (coeff_i) * geosum(10^6, j*T15)
    //
    // Sum over all periods j = 0..J-1 for slot i:
    //   J * fixed_i + coeff_i * sum_{j=0}^{J-1} geosum(10^6, j*T15)
    //
    // Now geosum(10^6, j*T15) = (10^(6*j*T15) - 1) / (10^6 - 1)
    // Let B = 10^(6*T15) mod MOD.
    // Then 10^(6*j*T15) = B^j.
    // sum_{j=0}^{J-1} geosum(10^6, j*T15) = sum_{j=0}^{J-1} (B^j - 1) / (10^6 - 1)
    //   = [geosum(B, J) - J] / (10^6 - 1)
    //   = [geosum(B, J) - J] * inv(10^6 - 1)
    //
    // Since gcd(10^6-1, MOD) was shown to be 1, the inverse exists.
    //
    // This gives us O(T) work per period plus O(log J) for the geometric sums.
    // Total: O(T + T * log(N)) which is very fast.

    // Let me implement this now.

    // Step 1: Find period T and record (p_i, r_i, q_i0) for each slot i in [0, T-1].
    int T;
    {
        int pos = 0;
        vector<int> block_pos;
        block_pos.push_back(pos);
        for (int k = 0; k < 120; k++) {
            for (int j = 1; j <= 15; j++) {
                int r = (15 * k + j) % 15;
                pos = part_end[pos][r];
            }
            block_pos.push_back(pos);
        }
        int per = 0;
        for (int p = 1; p <= 120; p++) {
            if (block_pos[p] == block_pos[0]) {
                bool ok = true;
                for (int i = 0; i + p <= 120; i++)
                    if (block_pos[i] != block_pos[i + p]) { ok = false; break; }
                if (ok) { per = p; break; }
            }
        }
        T = per * 15;
    }
    printf("Period T = %d\n", T);

    int T15 = T / 15;

    // Step 2: Record slot info
    struct SlotInfo {
        int pos;  // starting position in cycle
        int r;    // n % 15
        ll q0;    // (i+1) / 15, base q value
        ll fixed_val; // C_i * P_i * geosum(10^6, q0) + V_i (mod MOD)
        ll coeff;     // C_i * P_i * 10^(6*q0) (mod MOD)
    };

    vector<SlotInfo> slots(T);
    {
        int pos = 0;
        for (int i = 0; i < T; i++) {
            int n = i + 1;
            int r = n % 15;
            ll q0 = n / 15;

            SlotInfo si;
            si.pos = pos;
            si.r = r;
            si.q0 = q0;

            ll C = cycle_val[pos];
            ll L = part_dig[pos][r];
            ll P = power(10, L, MOD);
            ll V = part_val[pos][r];
            ll G_q0 = geosum(power(10, 6, MOD), q0, MOD);

            si.fixed_val = (C % MOD * P % MOD % MOD * G_q0 % MOD + V) % MOD;
            ll pow6q0 = power(10, 6 * q0, MOD);
            si.coeff = C % MOD * P % MOD % MOD * pow6q0 % MOD;

            slots[i] = si;

            pos = part_end[pos][r];
        }
    }

    // Step 3: Compute the answer
    ll N = 100000000000000LL; // 10^14

    // Number of complete periods
    ll J = N / T;
    ll remainder = N % T; // remaining slots in last incomplete period

    // For complete periods (j = 0..J-1):
    // Sum = sum_{i=0}^{T-1} [J * fixed_i + coeff_i * SUM_GEO]
    // where SUM_GEO = sum_{j=0}^{J-1} geosum(10^6, j*T15)
    //               = sum_{j=0}^{J-1} (10^(6*j*T15) - 1) / (10^6 - 1)
    //               = [sum_{j=0}^{J-1} B^j - J] / (10^6 - 1)
    //               = [geosum(B, J) - J] * inv(10^6 - 1)

    ll P6 = power(10, 6, MOD);
    ll B = power(10, (ll)6 * T15, MOD); // 10^(6*T15) mod MOD
    ll inv_p6m1 = power(P6 - 1 + MOD, MOD - 2, MOD);
    // Wait, MOD is not prime (41 * 3011081). Can't use Fermat's little theorem.
    // Need extended GCD.

    // Extended GCD for modular inverse
    auto ext_gcd = [](ll a, ll b, ll &x, ll &y) -> ll {
        if (b == 0) { x = 1; y = 0; return a; }
        ll x1, y1;
        ll g = 0;
        // Iterative version
        ll old_r = a, r = b;
        ll old_s = 1, s = 0;
        ll old_t = 0, t = 1;
        while (r != 0) {
            ll q = old_r / r;
            ll temp;
            temp = old_r - q * r; old_r = r; r = temp;
            temp = old_s - q * s; old_s = s; s = temp;
            temp = old_t - q * t; old_t = t; t = temp;
        }
        x = old_s; y = old_t;
        return old_r;
    };

    auto modinv = [&](ll a, ll m) -> ll {
        ll x, y;
        ll g = ext_gcd(((a % m) + m) % m, m, x, y);
        if (g != 1) return -1; // no inverse
        return ((x % m) + m) % m;
    };

    inv_p6m1 = modinv(P6 - 1, MOD);
    if (inv_p6m1 == -1) {
        printf("No inverse for 10^6 - 1 mod MOD\n");
        return 1;
    }

    // Recompute geosum for potentially non-prime MOD
    // Our geosum function works fine for any MOD since it uses divide-and-conquer.

    // For B = 10^(6*T15), compute geosum(B, J)
    ll geo_B_J = geosum(B, J, MOD);
    // SUM_GEO = (geo_B_J - J%MOD) * inv_p6m1 mod MOD
    ll SUM_GEO = mod(mod(geo_B_J - J % MOD + MOD) * inv_p6m1);

    ll total_fixed = 0;
    ll total_coeff = 0;
    for (int i = 0; i < T; i++) {
        total_fixed = (total_fixed + slots[i].fixed_val) % MOD;
        total_coeff = (total_coeff + slots[i].coeff) % MOD;
    }

    ll ans = mod(J % MOD * total_fixed % MOD + total_coeff * SUM_GEO % MOD);

    // Step 4: Handle remaining slots (last incomplete period, j = J)
    for (ll i = 0; i < remainder; i++) {
        ll n = J * T + i + 1;
        int r = slots[i].r;
        int pos = slots[i].pos;
        ll q = n / 15;

        ll C = cycle_val[pos];
        ll L = part_dig[pos][r];
        ll P = power(10, L, MOD);
        ll V = part_val[pos][r];
        ll G_q = geosum(P6, q, MOD);

        ll val = mod(C * P % MOD * G_q % MOD + V);
        ans = (ans + val) % MOD;
    }

    printf("S(10^14) mod %lld = %lld\n", MOD, ans);

    return 0;
}
