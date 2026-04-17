#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 304: Primonacci
 *
 * Find sum of F(p_{10^14+i}) mod 1234567891011 for i=1..100000.
 * p_k is the k-th prime, F(n) is the n-th Fibonacci number.
 *
 * Steps:
 * 1. Use Lucy_Hedgehog to compute pi(x) and binary search for a point near
 *    the 10^14-th prime.
 * 2. Segmented sieve to collect 100000 consecutive primes from that point.
 * 3. Matrix exponentiation for Fibonacci with incremental gaps.
 */

typedef long long ll;
typedef __int128 lll;

const ll MOD = 1234567891011LL;

struct Mat { ll a[2][2]; };

Mat mat_mul(const Mat& A, const Mat& B, ll m) {
    Mat C;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            lll s = 0;
            for (int k = 0; k < 2; k++)
                s += (lll)A.a[i][k] * B.a[k][j];
            C.a[i][j] = (ll)(s % m);
        }
    return C;
}

Mat mat_pow(Mat M, ll n, ll m) {
    Mat R = {{{1,0},{0,1}}};
    while (n > 0) {
        if (n & 1) R = mat_mul(R, M, m);
        M = mat_mul(M, M, m);
        n >>= 1;
    }
    return R;
}

vector<int> small_primes;

void gen_small_primes(int lim) {
    vector<bool> sieve(lim + 1, false);
    for (int i = 2; i <= lim; i++) {
        if (!sieve[i]) {
            small_primes.push_back(i);
            for (ll j = (ll)i * i; j <= lim; j += i)
                sieve[j] = true;
        }
    }
}

vector<ll> seg_sieve(ll lo, ll hi) {
    if (lo < 2) lo = 2;
    int sz = (int)(hi - lo + 1);
    vector<bool> is_composite(sz, false);
    for (int p : small_primes) {
        if ((ll)p * p > hi) break;
        ll start = max((ll)p * p, ((lo + p - 1) / p) * p);
        for (ll j = start; j <= hi; j += p)
            is_composite[(int)(j - lo)] = true;
    }
    vector<ll> result;
    for (int i = 0; i < sz; i++)
        if (!is_composite[i])
            result.push_back(lo + i);
    return result;
}

ll prime_count(ll n) {
    if (n < 2) return 0;
    ll sqrtn = (ll)sqrt((double)n);
    while ((sqrtn + 1) * (sqrtn + 1) <= n) sqrtn++;
    while (sqrtn * sqrtn > n) sqrtn--;

    int sz = (int)sqrtn;
    int* Ssmall = new int[sz + 2];
    ll* Slarge = new ll[sz + 2];

    Ssmall[0] = 0; Slarge[0] = 0;
    for (int i = 1; i <= sz; i++) {
        Ssmall[i] = i - 1;
        Slarge[i] = n / i - 1;
    }

    for (ll p = 2; p <= sqrtn; p++) {
        if (Ssmall[p] == Ssmall[p - 1]) continue;
        int pcnt = Ssmall[p - 1];
        ll p2 = p * p;
        ll lim = min((ll)sz, n / p2);
        for (ll i = 1; i <= lim; i++) {
            ll d = i * p;
            ll val = (d <= sz) ? Slarge[d] : Ssmall[n / d];
            Slarge[i] -= (val - pcnt);
        }
        for (ll i = sz; i >= p2; i--) {
            Ssmall[i] -= (Ssmall[i / p] - pcnt);
        }
    }

    ll result = Slarge[1];
    delete[] Ssmall;
    delete[] Slarge;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll target = 100000000000000LL; // 10^14

    // Need primes up to sqrt(4e15) ~ 63M
    gen_small_primes(64000000);

    // Binary search for x such that pi(x) is close to target.
    // p_{10^14} ~ 10^14 * (ln(10^14) + ln(ln(10^14))) ~ 10^14 * 35.7 ~ 3.57e15
    //
    // We do binary search, calling prime_count at the midpoint each time.
    // Each call takes ~2-3 minutes for x ~ 3.5e15. With ~30 bisection steps this is too slow.
    //
    // Instead: call prime_count ONCE at a good estimate, then use segmented sieve
    // to adjust by at most ~10^8 numbers.

    // Better estimate using Cipolla's asymptotic: p_n ~ n(ln n + ln ln n - 1 + (ln ln n - 2)/ln n)
    // For n = 10^14:
    // ln(10^14) = 32.2362
    // ln(ln(10^14)) = ln(32.2362) = 3.4730
    // p ~ 10^14 * (32.2362 + 3.4730 - 1 + (3.4730 - 2)/32.2362)
    //   = 10^14 * (34.7092 + 0.04569)
    //   = 10^14 * 34.7549
    //   = 3.47549e15

    // Let's try a single prime_count call at 3.4755e15
    ll x0 = 3475500000000000LL;

    fprintf(stderr, "Computing pi(%lld)...\n", x0);
    ll pi0 = prime_count(x0);
    fprintf(stderr, "pi(%lld) = %lld, target = %lld, diff = %lld\n",
            x0, pi0, target, target - pi0);

    // diff = target - pi0. Each prime gap ~ ln(x0) ~ 35.8
    // Adjust: approximately diff * 36 positions forward (or back if negative)
    ll diff = target - pi0;
    ll gap_est = 36;

    // If |diff| is small enough (< ~3M primes ~ 10^8 positions), we can sieve
    // Otherwise, do another prime_count call at x0 + diff*gap_est

    if (abs(diff) > 3000000LL) {
        ll x1 = x0 + diff * gap_est;
        fprintf(stderr, "Computing pi(%lld)...\n", x1);
        ll pi1 = prime_count(x1);
        fprintf(stderr, "pi(%lld) = %lld, diff = %lld\n", x1, pi1, target - pi1);
        x0 = x1;
        pi0 = pi1;
        diff = target - pi0;
    }

    if (abs(diff) > 3000000LL) {
        // One more iteration
        ll x1 = x0 + diff * gap_est;
        fprintf(stderr, "Computing pi(%lld)...\n", x1);
        ll pi1 = prime_count(x1);
        fprintf(stderr, "pi(%lld) = %lld, diff = %lld\n", x1, pi1, target - pi1);
        x0 = x1;
        pi0 = pi1;
        diff = target - pi0;
    }

    // Now |diff| should be small. Sieve forward/backward.
    ll sieve_start;
    ll primes_to_skip;

    if (diff >= 0) {
        sieve_start = x0 + 1;
        primes_to_skip = diff;
    } else {
        // Go back: |diff| primes * ~36 + safety margin
        sieve_start = x0 + diff * gap_est - 5000000;
        if (sieve_start < 2) sieve_start = 2;

        // Count primes in [sieve_start, x0] to find pi(sieve_start-1)
        ll count_in_range = 0;
        ll seg_sz = 10000000;
        for (ll lo = sieve_start; lo <= x0; lo += seg_sz) {
            ll hi = min(lo + seg_sz - 1, x0);
            auto ps = seg_sieve(lo, hi);
            count_in_range += ps.size();
        }
        // pi(sieve_start - 1) = pi0 - count_in_range
        primes_to_skip = target - (pi0 - count_in_range);
    }

    fprintf(stderr, "Sieve from %lld, skip %lld primes, collect 100000\n",
            sieve_start, primes_to_skip);

    // Collect 100000 primes
    vector<ll> target_primes;
    target_primes.reserve(100000);
    ll seg_size = 10000000;
    ll sieve_lo = sieve_start;

    while ((ll)target_primes.size() < 100000) {
        ll sieve_hi = sieve_lo + seg_size - 1;
        auto primes_seg = seg_sieve(sieve_lo, sieve_hi);

        for (ll p : primes_seg) {
            if (primes_to_skip > 0) {
                primes_to_skip--;
            } else {
                target_primes.push_back(p);
                if ((ll)target_primes.size() >= 100000) break;
            }
        }
        sieve_lo = sieve_hi + 1;
    }

    fprintf(stderr, "Collected %zu primes: [%lld, %lld]\n",
            target_primes.size(), target_primes.front(), target_primes.back());

    // Compute sum of F(p_i) mod MOD
    Mat M = {{{1,1},{1,0}}};
    Mat cur = mat_pow(M, target_primes[0] - 1, MOD);
    ll total = cur.a[0][0] % MOD;

    for (int i = 1; i < (int)target_primes.size(); i++) {
        ll g = target_primes[i] - target_primes[i-1];
        Mat Mg = mat_pow(M, g, MOD);
        cur = mat_mul(cur, Mg, MOD);
        total = (total + cur.a[0][0]) % MOD;
    }

    cout << total << endl;
    return 0;
}
