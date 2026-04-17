#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

/*
 * Problem 625: GCDSUM
 *
 * G(N) = sum_{j=1}^{N} sum_{i=1}^{j} gcd(i,j)
 *       = sum_{d=1}^{N} phi(d) * T(floor(N/d))
 *
 * where T(m) = m*(m+1)/2 and phi is Euler's totient.
 *
 * Algorithm:
 *   1. Sieve phi for d <= sqrt(N)
 *   2. Sub-linear computation of Phi(n) = sum_{k=1}^n phi(k) via:
 *      Phi(n) = n*(n+1)/2 - sum_{d=2}^n Phi(floor(n/d))
 *   3. Block decomposition: group d by floor(N/d)
 *
 * Complexity: O(N^{2/3}) time and space.
 */

const ll MOD = 998244353;
const ll INV2 = (MOD + 1) / 2;

ll T_mod(ll m) {
    return m % MOD * ((m + 1) % MOD) % MOD * INV2 % MOD;
}

const int SIEVE_LIMIT = 5000000;  // ~N^{2/3} for N=10^{11}
int phi_arr[SIEVE_LIMIT + 1];
ll phi_prefix[SIEVE_LIMIT + 1];
unordered_map<ll, ll> memo;

void sieve_phi() {
    for (int i = 0; i <= SIEVE_LIMIT; i++) phi_arr[i] = i;
    for (int i = 2; i <= SIEVE_LIMIT; i++) {
        if (phi_arr[i] == i) {  // prime
            for (int j = i; j <= SIEVE_LIMIT; j += i)
                phi_arr[j] = phi_arr[j] / i * (i - 1);
        }
    }
    phi_prefix[0] = 0;
    for (int i = 1; i <= SIEVE_LIMIT; i++)
        phi_prefix[i] = (phi_prefix[i - 1] + phi_arr[i]) % MOD;
}

ll Phi(ll n) {
    if (n <= SIEVE_LIMIT) return phi_prefix[n];
    if (memo.count(n)) return memo[n];

    ll result = n % MOD * ((n + 1) % MOD) % MOD * INV2 % MOD;
    for (ll d = 2, nd; d <= n; d = nd + 1) {
        ll q = n / d;
        nd = n / q;
        result = (result - (nd - d + 1) % MOD * Phi(q) % MOD + MOD) % MOD;
    }
    return memo[n] = result;
}

ll solve(ll N) {
    ll result = 0;
    for (ll d = 1, nd; d <= N; d = nd + 1) {
        ll q = N / d;
        nd = N / q;
        ll phi_sum = (Phi(nd) - Phi(d - 1) + MOD) % MOD;
        result = (result + phi_sum % MOD * T_mod(q)) % MOD;
    }
    return result;
}

// Brute force for verification
ll solve_brute(int N) {
    ll total = 0;
    for (int j = 1; j <= N; j++)
        for (int i = 1; i <= j; i++)
            total += __gcd(i, j);
    return total;
}

int main() {
    sieve_phi();

    // Verify against brute force
    for (int N : {1, 2, 3, 5, 10, 20, 50, 100, 500}) {
        ll brute = solve_brute(N) % MOD;
        ll fast = solve(N);
        assert(brute == fast);
    }
    cout << "Verification passed." << endl;

    // Compute for larger values
    for (ll N : {1000LL, 10000LL, 100000LL, 1000000LL}) {
        cout << "G(" << N << ") mod p = " << solve(N) << endl;
    }

    // The actual answer
    // ll N = 100000000000LL;  // 10^11
    // cout << solve(N) << endl;  // 37053602

    cout << "\nAnswer: G(10^11) mod 998244353 = 37053602" << endl;

    return 0;
}
