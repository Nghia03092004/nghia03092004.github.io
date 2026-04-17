/*
 * Project Euler Problem 412: Gnomon Numbering
 *
 * Compute LC(10000, 5000) mod 76543217 using the hook length formula
 * for Standard Young Tableaux of the L-shaped partition.
 *
 * Partition: lambda = (m^{m-n}, (m-n)^n)
 * Hook length formula: f^lambda = N! / prod h(i,j)
 *
 * The hooks decompose into three rectangular regions, each row
 * contributing a ratio of consecutive factorials.
 *
 * Answer: 38788800
 *
 * Compile: g++ -O2 -o solution solution.cpp
 */

#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

const ll MOD = 76543217;

ll power(ll base, ll exp, ll mod) {
    ll result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

ll solve(int m, int n, ll p) {
    int a = m - n;
    ll N = (ll)m * m - (ll)n * n;

    ll max_val = max(N, (ll)(2 * m)) + 10;

    // Precompute factorials mod p
    vector<ll> fact(max_val + 1), inv_fact(max_val + 1);
    fact[0] = 1;
    for (ll i = 1; i <= max_val; i++)
        fact[i] = fact[i - 1] * i % p;

    inv_fact[max_val] = power(fact[max_val], p - 2, p);
    for (ll i = max_val - 1; i >= 0; i--)
        inv_fact[i] = inv_fact[i + 1] * (i + 1) % p;

    ll numerator = fact[N];
    ll denom = 1;

    // Region 1: rows [0, a), cols [0, a)
    // Row i hook product = fact[2m-i-1] * inv_fact[2m-i-a-1]
    for (int i = 0; i < a; i++) {
        int top = 2 * m - i - 1;
        int bot = 2 * m - i - a - 1;
        denom = denom * fact[top] % p * inv_fact[bot] % p;
    }

    // Region 2: rows [0, a), cols [a, m)
    // Row i hook product = fact[m-i-1] * inv_fact[a-i-1]
    for (int i = 0; i < a; i++) {
        int top = m - i - 1;
        int bot = a - i - 1;
        denom = denom * fact[top] % p * inv_fact[bot] % p;
    }

    // Region 3: rows [a, m), cols [0, a)
    // Row i hook product = fact[a+m-i-1] * inv_fact[m-i-1]
    for (int i = a; i < m; i++) {
        int top = a + m - i - 1;
        int bot = m - i - 1;
        denom = denom * fact[top] % p * inv_fact[bot] % p;
    }

    return numerator % p * power(denom, p - 2, p) % p;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll p = MOD;

    // Verification
    cout << "LC(3, 0) = " << solve(3, 0, p) << " (expected 42)" << endl;
    cout << "LC(5, 3) = " << solve(5, 3, p) << " (expected 250250)" << endl;
    cout << "LC(10, 5) = " << solve(10, 5, p) << " (expected 61251715)" << endl;

    // Main problem
    cout << endl;
    cout << "LC(10000, 5000) mod " << p << " = " << solve(10000, 5000, p) << endl;

    return 0;
}
