#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;

const ll MOD = 100000;

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

// Product of k for k=1..n where gcd(k,5)=1 and gcd(k,2)=1, mod 10^5
// Actually we need product of k for k=1..n with factors of 2 and 5 stripped

// Precompute partial products of numbers coprime to 10 in [1..10^5]
ll partial[100001]; // partial[i] = product of {k : 1<=k<=i, gcd(k,10)=1} mod 10^5

void precompute() {
    partial[0] = 1;
    for (int i = 1; i <= 100000; i++) {
        if (i % 2 != 0 && i % 5 != 0)
            partial[i] = partial[i-1] * i % MOD;
        else
            partial[i] = partial[i-1];
    }
}

// Product of numbers coprime to 10 in [1..n], mod 10^5
ll coprime_product(ll n) {
    if (n <= 0) return 1;
    ll full_blocks = n / MOD;
    ll remainder = n % MOD;
    // Product over full blocks of [1..10^5]: each block contributes partial[MOD]
    // partial[MOD] = product of units mod 10^5 = 99999 (which is -1 mod 10^5)
    ll result = power(partial[100000], full_blocks, MOD);
    result = result * partial[remainder] % MOD;
    return result;
}

// Compute the "odd part" (with respect to 5) of n!, mod 10^5
// This is: product of all k in [1..n] with all factors of 5 removed, but keeping factors of 2
// We separate: product of k coprime to 5, times (product for floor(n/5) recursively)
// But we also need to handle factor 2 separately. Let's compute the product
// with ALL factors of 2 and 5 removed.

// f(n) = product of (k with 2 and 5 removed) for k=1..n, mod 10^5
// = coprime_product(n) * f(floor(n/2)) * f(floor(n/5))... no, that's wrong.

// Better approach:
// Let g(n) = product of (k / 2^v2(k) / 5^v5(k)) for k=1..n, mod 10^5
//
// Alternative: factor out 5s level by level.
// product_{k=1}^{n} k = (product of k coprime to 5 in [1..n]) * 5^{v5(n!)} * (floor(n/5))!
//
// So the "5-free" part: remove all 5s from n!
// Let h(n) = n! / 5^{v5(n!)} mod (something)
// h(n) = (product of k coprime to 5 in [1..n]) * h(floor(n/5))
//
// But this still has factors of 2 in it.
//
// We want n! / (2^a * 5^b) mod 10^5, where a = v2(n!), b = v5(n!).
//
// Strategy:
// 1. Compute n! / 5^b mod 2^5: since n! / 5^b is divisible by huge power of 2,
//    n! / (2^a * 5^b) is odd. Mod 2^5 = 32, we need this odd number mod 32.
// 2. Compute n! / 5^b mod 5^5: strip out 5s recursively.
// 3. Combine via CRT.
// 4. Multiply by 2^(a-b) mod 10^5.

// Actually let's think differently.
// We want: n! / 10^b mod 10^5, where b = v5(n!).
// n! / 10^b = n! / (2^b * 5^b) * ... wait no: 10^b = 2^b * 5^b.
// So n! / 10^b = (n! / (2^b * 5^b)).
// But n! = 2^a * 5^b * m where gcd(m, 10) = 1 and a > b.
// So n! / 10^b = 2^(a-b) * m.
// We need 2^(a-b) * m mod 10^5.

// Step 1: compute a = v2(n!), b = v5(n!)
ll padic_val(ll n, ll p) {
    ll count = 0;
    ll pk = p;
    while (pk <= n) {
        count += n / pk;
        pk *= p;
    }
    return count;
}

// Step 2: compute m = n! / (2^a * 5^b) mod 5^5
// m mod 5^5: since all 2s and 5s are removed, gcd(m, 10) = 1.
//
// To compute m mod 5^5 = 3125:
// Use the formula for n! mod p^k (removing factors of p) from Andrew Granville /
// generalized Wilson's theorem.
//
// Product of numbers 1..n with all factors of 5 removed, then remove all factors of 2.
//
// Let's use a different approach. Compute n!/(5^b) mod 5^5 using the recursive method,
// then divide out 2^a mod 5^5.

const ll MOD5 = 3125; // 5^5

// Product of k for k=1..n, gcd(k,5)=1, mod 5^5
ll partial5[3126];

void precompute5() {
    partial5[0] = 1;
    for (int i = 1; i <= 3125; i++) {
        if (i % 5 != 0)
            partial5[i] = partial5[i-1] * i % MOD5;
        else
            partial5[i] = partial5[i-1];
    }
}

ll coprime5_product(ll n) {
    if (n <= 0) return 1;
    ll full = n / MOD5;
    ll rem = n % MOD5;
    // partial5[3125] = product of units mod 5^5 = -1 mod 3125 = 3124
    ll result = power(partial5[3125], full, MOD5);
    result = result * partial5[rem] % MOD5;
    return result;
}

// n! / 5^{v5(n!)} mod 5^5
ll fact_remove5(ll n) {
    if (n <= 1) return 1;
    return coprime5_product(n) * fact_remove5(n / 5) % MOD5;
}

// Now for mod 2^5 = 32:
const ll MOD2 = 32;

// n! / 2^{v2(n!)} mod 32
ll partial2[33];

void precompute2() {
    partial2[0] = 1;
    for (int i = 1; i <= 32; i++) {
        int x = i;
        while (x % 2 == 0) x /= 2;
        partial2[i] = partial2[i-1] * x % MOD2;
    }
}

ll coprime2_product(ll n) {
    if (n <= 0) return 1;
    ll full = n / MOD2;
    ll rem = n % MOD2;
    // Product of odd numbers in [1..32] mod 32
    // partial2[32] should be the product with 2s removed
    ll result = power(partial2[32], full, MOD2);
    result = result * partial2[rem] % MOD2;
    return result;
}

ll fact_remove2(ll n) {
    if (n <= 1) return 1;
    return coprime2_product(n) * fact_remove2(n / 2) % MOD2;
}

// Extended GCD
ll extgcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) { x = 1; y = 0; return a; }
    ll x1, y1;
    ll g = extgcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - (a / b) * y1;
    return g;
}

ll modinv(ll a, ll m) {
    ll x, y;
    extgcd(a, m, x, y);
    return (x % m + m) % m;
}

// CRT: find x such that x = a1 mod m1, x = a2 mod m2
ll crt(ll a1, ll m1, ll a2, ll m2) {
    // x = a1 + m1 * t, a1 + m1*t = a2 (mod m2)
    // t = (a2 - a1) * m1^{-1} mod m2
    ll t = (a2 - a1 % m2 + m2) % m2 * modinv(m1 % m2, m2) % m2;
    return a1 + m1 * t;
}

int main() {
    precompute();
    precompute5();
    precompute2();

    ll N = 1000000000000LL; // 10^12

    ll a = padic_val(N, 2); // v2(N!)
    ll b = padic_val(N, 5); // v5(N!)

    // m = N! / (2^a * 5^b) mod 10^5
    // Compute mod 5^5 and mod 2^5 separately

    // mod 5^5: m mod 3125
    // N! / 5^b mod 5^5 = fact_remove5(N)
    // N! / (2^a * 5^b) mod 5^5 = fact_remove5(N) * modinv(2^a mod 3125, 3125) mod 3125
    // But 2^a mod 3125: use power(2, a, 3125)
    ll fr5 = fact_remove5(N);
    ll inv2a_mod5 = modinv(power(2, a % 2500, MOD5), MOD5);
    // Euler's totient of 3125 = 3125*(1-1/5) = 2500, so 2^a mod 3125 has period 2500
    ll m_mod5 = fr5 * inv2a_mod5 % MOD5;

    // mod 2^5: m mod 32
    // N! / 2^a mod 32 = fact_remove2(N)
    // N! / (2^a * 5^b) mod 32 = fact_remove2(N) * modinv(5^b mod 32, 32) mod 32
    ll fr2 = fact_remove2(N);
    ll inv5b_mod2 = modinv(power(5, b % 16, MOD2), MOD2);
    // Euler's totient of 32 = 16
    ll m_mod2 = fr2 * inv5b_mod2 % MOD2;

    // CRT to get m mod 10^5
    ll m = crt(m_mod2, MOD2, m_mod5, MOD5);

    // Answer = 2^(a-b) * m mod 10^5
    // But we need to handle 2^(a-b) mod 10^5
    // mod 5^5: power(2, a-b, 3125)
    // mod 2^5: 2^(a-b) mod 32. Since a-b is large (a >> b), 2^(a-b) mod 32 = 0.
    // Wait, a-b for N=10^12: a = sum floor(10^12/2^i) ~ 10^12, b = sum floor(10^12/5^i) ~ 2.5*10^11.
    // a - b ~ 7.5 * 10^11, so 2^(a-b) mod 32 = 0 since a-b >= 5.

    // So the answer mod 32 = 0 * m mod 32... wait that's not right.
    //
    // Actually: answer = N! / 10^b mod 10^5 = 2^(a-b) * m mod 10^5
    // where m is odd (coprime to 10).
    //
    // 2^(a-b) mod 32: since a-b >= 5, this is 0 mod 32.
    // But the answer can't be 0 mod 32...
    //
    // Hmm, the issue is that the answer = N!/10^b, and N!/10^b may still be divisible by 2.
    // The "last 5 non-trailing-zero digits" means we strip trailing zeros (factors of 10),
    // then take mod 10^5. The result can be even -- it just can't end in 0 (be divisible by 10).
    //
    // So the answer IS 2^(a-b) * m mod 10^5, and it can be even.

    // To compute 2^(a-b) * m mod 10^5:
    // mod 5^5: power(2, a-b, 3125) * m_mod5 mod 3125
    // But wait, m_mod5 was already m mod 3125 (with 2^a divided out).
    // Let me redo this more carefully.

    // Let R = N! / 10^b. We want R mod 10^5.
    // R = N! / (2^b * 5^b) = 2^(a-b) * (N! / (2^a * 5^b))
    //
    // R mod 5^5:
    //   N!/5^b mod 5^5 = fact_remove5(N) [product with 5s stripped]
    //   N!/(2^b * 5^b) mod 5^5 = fact_remove5(N) * inv(2^b) mod 5^5
    //   NO WAIT: fact_remove5 strips ALL factors of 5, giving N!/(5^b) mod 5^5.
    //   But 2^b is still in there. So:
    //   R mod 5^5 = (N!/5^b mod 5^5) * inv(2^b) mod 5^5... no.
    //   R = N!/(2^b * 5^b).
    //   R mod 5^5 = [N!/5^b mod 5^5] * [inv(2^b mod 5^5)] mod 5^5... no, we can't just do that.
    //   N!/5^b mod 5^5 = fact_remove5(N). This has all 2 factors still in it.
    //   But 2 is invertible mod 5^5, so:
    //   R mod 5^5 = fact_remove5(N) * modinv(power(2, b, 3125), 3125) mod 3125
    //   Then 2^(a-b) is NOT separated, it's already included in fact_remove5(N)/2^b.
    //   Actually: fact_remove5(N) = N!/5^b mod 5^5 = 2^a * m * (other odd non-5 factors) mod 5^5.
    //   Hmm, fact_remove5(N) computes the product of (k with 5s removed) for k=1..N, mod 5^5.
    //   That includes ALL factors of 2. So fact_remove5(N) = (N!/5^b) mod 5^5, and
    //   R = N!/(2^b * 5^b), so R mod 5^5 = fact_remove5(N) * modinv(2^b, 5^5) mod 5^5.
    //   WRONG: R = N!/10^b, so R mod 5^5 = (N!/5^b) * (1/2^b) mod 5^5 only if 2^b is coprime to 5^5, which it is.
    //   Actually no: N!/10^b = N!/(2^b * 5^b). And N!/5^b = fact_remove5(N) mod 5^5.
    //   So (N!/5^b) / 2^b = N!/10^b. And mod 5^5, since gcd(2,5)=1:
    //   R mod 5^5 = fact_remove5(N) * modinv(power(2, b % 2500, 3125), 3125) mod 3125.

    // R mod 2^5:
    //   R = N! / (2^b * 5^b).
    //   N! has 2^a factor. So R has 2^(a-b) factor.
    //   a - b for N=10^12: let me compute.
    //   Since a-b >= 5 for sure, R mod 32 depends on how many 2s beyond 5.
    //
    //   Let's compute carefully:
    //   R mod 32: R = 2^(a-b) * (N!/(2^a * 5^b))
    //   N!/(2^a * 5^b) = m * (product of odd numbers with 5s removed... wait,
    //   N!/(2^a) is the odd part of N! (w.r.t. 2), and then /5^b removes the 5s.
    //
    //   N!/(2^a) mod 32 = fact_remove2(N). This is odd.
    //   N!/(2^a * 5^b) mod 32 = fact_remove2(N) * modinv(5^b mod 32, 32) mod 32.
    //   This is odd too (since 5 is odd and fact_remove2 is odd).
    //
    //   R = 2^(a-b) * (N!/(2^a * 5^b))
    //   R mod 32: if a-b >= 5, then 2^(a-b) mod 32 = 0, so R mod 32 = 0.
    //   But that's only true if a-b >= 5 AND the odd part doesn't contribute.
    //   2^(a-b) * odd mod 32: if a-b >= 5, this is 0 mod 32.

    // So R mod 32 = 0 if a-b >= 5. Let me verify a-b >= 5 for N = 10^12.
    // a - b = v2(N!) - v5(N!) = sum floor(N/2^i) - sum floor(N/5^i)
    // >= N/2 - N/4 ... this is huge. Definitely >= 5.

    // R mod 5^5:
    ll R_mod5 = fr5 * modinv(power(2, b % 2500, MOD5), MOD5) % MOD5;

    // R mod 2^5:
    // R = 2^(a-b) * m', where m' is odd.
    // m' = fact_remove2(N) * modinv(5^b, 32) mod 32
    ll m_prime = fr2 * inv5b_mod2 % MOD2;
    // R mod 32 = 2^(a-b) * m_prime mod 32
    ll e = a - b;
    ll pow2e_mod32;
    if (e >= 5)
        pow2e_mod32 = 0;
    else
        pow2e_mod32 = (1LL << e);
    ll R_mod2 = pow2e_mod32 * m_prime % MOD2;

    // CRT
    ll answer = crt(R_mod2, MOD2, R_mod5, MOD5);

    cout << answer << endl;
    return 0;
}
