#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// Lucy Hedgehog's prime counting function
// Returns pi(x) for any x, after preprocessing for values up to N
struct PrimeCount {
    ll N;
    int sqrtN;
    vector<ll> small, large;
    vector<int> primes;
    vector<bool> sieve;

    void init(ll n) {
        N = n;
        sqrtN = (int)sqrt((double)n);
        while ((ll)sqrtN * sqrtN > n) sqrtN--;
        while ((ll)(sqrtN + 1) * (sqrtN + 1) <= n) sqrtN++;

        // Sieve primes up to sqrtN
        sieve.assign(sqrtN + 1, true);
        sieve[0] = sieve[1] = false;
        primes.clear();
        for (int i = 2; i <= sqrtN; i++) {
            if (sieve[i]) {
                primes.push_back(i);
                for (ll j = (ll)i * i; j <= sqrtN; j += i)
                    sieve[j] = false;
            }
        }

        // small[i] = count of primes <= i, for i = 0..sqrtN
        // large[i] = count of primes <= N/i, for i = 1..sqrtN
        small.assign(sqrtN + 1, 0);
        large.assign(sqrtN + 1, 0);

        for (int i = 1; i <= sqrtN; i++) {
            small[i] = i - 1; // initially, all numbers >= 2
            large[i] = N / i - 1;
        }

        for (int p : primes) {
            ll pp = (ll)p * p;
            if (pp > N) break;

            ll pcnt_prev = small[p - 1]; // pi(p-1)

            for (int i = 1; i <= sqrtN && (ll)i * pp <= N; i++) {
                ll d = (ll)i * p;
                if (d <= sqrtN)
                    large[i] -= large[d] - pcnt_prev;
                else
                    large[i] -= small[N / d] - pcnt_prev;
            }

            for (int i = sqrtN; i >= pp; i--) {
                small[i] -= small[i / p] - pcnt_prev;
            }
        }
    }

    ll count(ll x) {
        if (x <= 0) return 0;
        if (x <= sqrtN) return small[x];
        ll idx = N / x;
        if (idx <= sqrtN) return large[idx];
        // Fallback -- shouldn't happen if x is of form N/k
        return 0;
    }
};

int main() {
    ll N = 1000000000000LL; // 10^12

    PrimeCount pc;
    pc.init(N);

    ll ans = 0;

    // Case 1: p^7 <= N => p <= N^(1/7) ~ 51
    {
        int lim = (int)pow((double)N, 1.0 / 7.0);
        while ((ll)pow(lim + 1, 7) <= N) lim++;
        while ((ll)pow(lim, 7) > N) lim--;
        // Count primes up to lim
        for (int p : pc.primes) {
            if (p > lim) break;
            ll val = 1;
            bool ok = true;
            for (int j = 0; j < 7; j++) {
                val *= p;
                if (val > N) { ok = false; break; }
            }
            if (ok && val <= N) ans++;
        }
    }

    // Case 2: p^3 * q <= N, p != q
    {
        // Sieve primes up to cbrt(N) ~ 10^4 for p
        int plim = (int)cbrt((double)N);
        while ((ll)(plim + 1) * (plim + 1) * (plim + 1) <= N) plim++;

        // We need all primes up to plim (and beyond for q)
        // Use pc.primes for small primes, and pc.count for pi(x)
        int sieve_lim = max(plim + 1, pc.sqrtN);
        vector<bool> sv(sieve_lim + 1, true);
        sv[0] = sv[1] = false;
        for (int i = 2; (ll)i * i <= sieve_lim; i++)
            if (sv[i])
                for (int j = i * i; j <= sieve_lim; j += i)
                    sv[j] = false;

        for (int p = 2; p <= plim; p++) {
            if (!sv[p]) continue;
            ll p3 = (ll)p * p * p;
            if (p3 >= N) break;
            ll qlim = N / p3;
            ll cnt = pc.count(qlim);
            // Subtract 1 if p <= qlim (to remove q = p)
            if (p <= qlim) cnt--;
            ans += cnt;
        }
    }

    // Case 3: p * q * r <= N, p < q < r, all distinct primes
    {
        int sieve_lim = pc.sqrtN;
        vector<bool> sv(sieve_lim + 1, true);
        sv[0] = sv[1] = false;
        for (int i = 2; (ll)i * i <= sieve_lim; i++)
            if (sv[i])
                for (int j = i * i; j <= sieve_lim; j += i)
                    sv[j] = false;

        vector<int> all_primes;
        for (int i = 2; i <= sieve_lim; i++)
            if (sv[i]) all_primes.push_back(i);

        for (int i = 0; i < (int)all_primes.size(); i++) {
            ll p = all_primes[i];
            if (p * p * p >= N) break; // need q > p, r > q, so p*q*r > p^3
            // Actually p*(p+1)*(p+2) >= p^3 approximately

            for (int j = i + 1; j < (int)all_primes.size(); j++) {
                ll q = all_primes[j];
                ll pq = p * q;
                if (pq * (q + 1) > N) break; // r > q needed
                ll rlim = N / pq;
                // r > q, r prime
                ll cnt = pc.count(rlim) - pc.count(q);
                if (cnt > 0) ans += cnt;
            }
        }
    }

    cout << ans << endl;

    return 0;
}
