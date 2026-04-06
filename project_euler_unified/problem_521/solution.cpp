#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 521: Smallest Prime Factor
 *
 * S(N) = sum_{n=2}^{N} smpf(n) mod 10^9.
 *
 * Uses Lucy DP (prime-counting sieve) with composite-contribution
 * accumulation during the sieve phase.
 */

typedef long long ll;
const ll MOD = 1000000000LL;

int main() {
    ll N = 1000000000000LL; // 10^12

    ll sqrtN = (ll)sqrt((double)N);
    while ((sqrtN + 1) * (sqrtN + 1) <= N) sqrtN++;
    while (sqrtN * sqrtN > N) sqrtN--;

    // Collect distinct floor(N/i) values
    vector<ll> vals;
    for (ll i = 1; i <= N;) {
        ll v = N / i;
        vals.push_back(v);
        i = N / v + 1;
    }
    sort(vals.begin(), vals.end());
    vals.erase(unique(vals.begin(), vals.end()), vals.end());
    int sz = (int)vals.size();

    // Index mapping
    auto idx = [&](ll v) -> int {
        return (int)(lower_bound(vals.begin(), vals.end(), v) - vals.begin());
    };

    // S0[i] = count, S1[i] = sum (mod MOD)
    vector<ll> S0(sz), S1(sz);
    for (int i = 0; i < sz; i++) {
        ll v = vals[i];
        S0[i] = v - 1;
        ll vm = v % MOD;
        S1[i] = (vm * ((vm + 1) % MOD) % MOD * 500000000LL % MOD - 1 + MOD) % MOD;
    }

    // Lucy DP sieve + composite contribution accumulation
    ll result = 0;

    // We need a copy of S0 for the composite counting pass
    vector<ll> S0c = S0;

    // Sieve S0 and S1 fully to get prime count and prime sum
    for (ll p = 2; p <= sqrtN; p++) {
        int pi = idx(p);
        int pi1 = idx(p - 1);
        if (S0[pi] == S0[pi1]) continue; // not prime

        ll pc = S0[pi1];
        ll ps = S1[pi1];
        ll p2 = p * p;

        for (int i = sz - 1; i >= 0 && vals[i] >= p2; i--) {
            ll v = vals[i];
            int vi = idx(v / p);
            ll cnt = S0[vi] - pc;
            S0[i] -= cnt;
            S1[i] = ((S1[i] - (p % MOD) * ((S1[vi] - ps + MOD) % MOD) % MOD) % MOD + MOD) % MOD;
        }
    }

    // S1[idx(N)] = sum of primes <= N (mod MOD)
    result = S1[idx(N)];

    // Now compute composite contributions using S0c
    // Process primes in order; before sieving prime p from S0c,
    // accumulate composites with smpf = p
    for (ll p = 2; p <= sqrtN; p++) {
        int pi = idx(p);
        int pi1 = idx(p - 1);
        if (S0c[pi] == S0c[pi1]) continue;

        ll pc = S0c[pi1]; // pi(p-1)

        // Count of composites in [2,N] with smpf = p:
        // = |{m in [2, N/p] : smpf(m) >= p}|
        // = S0c[N/p] - pc
        ll vp = N / p;
        if (vp >= 2) {
            ll cnt = S0c[idx(vp)] - pc;
            result = (result + (p % MOD) * (cnt % MOD) % MOD) % MOD;
        }

        // Now sieve p out of S0c
        ll p2 = p * p;
        for (int i = sz - 1; i >= 0 && vals[i] >= p2; i--) {
            ll v = vals[i];
            int vi = idx(v / p);
            S0c[i] -= (S0c[vi] - pc);
        }
    }

    cout << (result % MOD + MOD) % MOD << endl;

    return 0;
}
