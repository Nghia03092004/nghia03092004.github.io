#include <bits/stdc++.h>
using namespace std;

/*
 * Problem 320: Factorials Divisible by a Huge Integer
 *
 * N(i) = smallest n such that (i!)^E | n!, where E = 1234567890.
 * S = sum of N(i) for i = 10 to 1000000, modulo 10^18.
 *
 * For each prime p, the constraint is: v_p(n!) >= E * v_p(i!)
 * where v_p(m!) = sum_{k>=1} floor(m/p^k) (Legendre's formula).
 * N(i) = max over primes p <= i of min_n(p, E * v_p(i!)).
 *
 * Key insight: N(i) is non-decreasing (each n_p(i) is non-decreasing in i),
 * so we can maintain a running max. We track target_p = E * v_p(i!) incrementally
 * and only update when i is divisible by p.
 */

typedef long long i64;
typedef __int128 i128;

const int MAXN = 1000001;
const i64 E = 1234567890LL;
const i64 MOD = 1000000000000000000LL; // 10^18

bool is_prime[MAXN];
int spf[MAXN]; // smallest prime factor

// Legendre's formula: v_p(n!)
i64 legendre(i64 n, i64 p) {
    i64 s = 0;
    i64 pk = p;
    while (pk <= n) {
        s += n / pk;
        if (pk > n / p) break;
        pk *= p;
    }
    return s;
}

// Smallest n such that v_p(n!) >= target
i64 min_n_for_target(i64 p, i64 target) {
    if (target <= 0) return 0;
    i64 lo = 0;
    // Upper bound: v_p(n!) ~ n/(p-1), so n ~ target*(p-1)
    // But need to be safe with overflow
    i128 hi128 = (i128)target * (i128)p;
    i64 hi = (hi128 > (i128)2e18) ? (i64)2e18 : (i64)hi128;

    while (lo < hi) {
        i64 mid = lo + (hi - lo) / 2;
        if (legendre(mid, p) >= target)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

int main() {
    // Sieve
    fill(is_prime, is_prime + MAXN, true);
    is_prime[0] = is_prime[1] = false;
    memset(spf, 0, sizeof(spf));
    for (int i = 2; i < MAXN; i++) {
        if (is_prime[i]) {
            spf[i] = i;
            for (long long j = (long long)i * i; j < MAXN; j += i) {
                is_prime[j] = false;
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    // Fix spf for numbers whose smallest prime factor is themselves
    for (int i = 2; i < MAXN; i++)
        if (spf[i] == 0) spf[i] = i; // i is prime

    // Collect primes and create index map
    vector<int> primes;
    unordered_map<int, int> pidx;
    for (int i = 2; i < MAXN; i++) {
        if (is_prime[i]) {
            pidx[i] = primes.size();
            primes.push_back(i);
        }
    }
    int num_primes = primes.size();

    // For each prime, track current target and current n_p
    vector<i64> target_arr(num_primes, 0);
    vector<i64> np_arr(num_primes, 0);

    i64 current_max = 0;
    i64 S = 0;

    for (int i = 2; i <= 1000000; i++) {
        // Factorize i and update all prime factors
        int temp = i;
        while (temp > 1) {
            int p = spf[temp];
            int idx = pidx[p];
            int vp = 0;
            while (temp % p == 0) {
                temp /= p;
                vp++;
            }
            // v_p(i!) = v_p((i-1)!) + v_p(i), so target increases by E * v_p(i)
            target_arr[idx] += (i64)E * vp;
            np_arr[idx] = min_n_for_target(p, target_arr[idx]);
            if (np_arr[idx] > current_max) current_max = np_arr[idx];
        }

        if (i >= 10) {
            S = (S + current_max % MOD) % MOD;
        }
    }

    cout << S << endl;
    return 0;
}
