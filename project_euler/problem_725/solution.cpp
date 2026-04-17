#include <bits/stdc++.h>
using namespace std;

// We work modulo 10^16
const long long MOD = 10000000000000000LL; // 10^16

// Use __int128 for intermediate products to avoid overflow
typedef long long ll;
typedef __int128 lll;

ll mod(ll x) {
    return ((x % MOD) + MOD) % MOD;
}

ll mulmod(ll a, ll b) {
    return (lll)a % MOD * (b % MOD) % MOD;
}

ll addmod(ll a, ll b) {
    return (a + b) % MOD;
}

// Compute C(n, k) mod MOD for small k (k <= 9)
// n can be large (up to ~2028)
ll comb(ll n, int k) {
    if (k < 0 || n < k) return 0;
    if (k == 0) return 1;
    // C(n,k) = n*(n-1)*...*(n-k+1) / k!
    // Since k <= 9, k! is small and we can compute directly
    // But we need modular inverse of k! mod 10^16
    // 10^16 is not prime, so we need to be careful
    // Since k! divides the numerator product, we can compute exactly
    // Actually for k<=9, k! = at most 362880, and we're computing
    // product of k consecutive integers which is always divisible by k!
    // So we can do exact division in __int128
    lll num = 1;
    for (int i = 0; i < k; i++) {
        num *= (n - i);
    }
    // Divide by k!
    lll fact = 1;
    for (int i = 1; i <= k; i++) fact *= i;
    num /= fact;
    // Now take mod
    ll result = (ll)(num % MOD);
    if (result < 0) result += MOD;
    return result;
}

// f(s, k) = C(s+k-1, k-1) for s <= 9 (no inclusion-exclusion needed)
ll f(int s, int k) {
    if (s < 0 || k <= 0) return 0;
    return comb((ll)s + k - 1, k - 1);
}

// Modular exponentiation
ll powmod(ll base, ll exp, ll m) {
    ll result = 1;
    base %= m;
    while (exp > 0) {
        if (exp & 1) result = (lll)result * base % m;
        base = (lll)base * base % m;
        exp >>= 1;
    }
    return result;
}

// Sum of 10^0 + 10^1 + ... + 10^(n-1) mod MOD
// = (10^n - 1) / 9
// Since gcd(9, 10^16) = 1, we can find inverse of 9
// Actually 9 and 10^16 are coprime (9 = 3^2, 10^16 = 2^16 * 5^16)
// Use extended Euclidean to find inverse of 9 mod 10^16
ll inv9;

ll geometric_sum(int n) {
    // (10^n - 1) / 9 mod MOD
    if (n <= 0) return 0;
    if (n >= 16) {
        // 10^n mod 10^16 = 0 for n >= 16
        // (0 - 1) / 9 mod MOD = (-1) * inv9 mod MOD = MOD - inv9
        return (MOD - inv9) % MOD;
    }
    ll pw = 1;
    for (int i = 0; i < n; i++) pw *= 10;
    return (pw - 1) / 9; // exact division since n < 16
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
    ll g = extgcd(a, m, x, y);
    if (g != 1) return -1;
    return ((x % m) + m) % m;
}

int main() {
    int N = 2020;

    inv9 = modinv(9, MOD);

    ll answer = 0;

    // For each number of digits m (2 to N):
    // For each special digit d (1 to 9):
    // The remaining k = m-1 digits must sum to d
    // f(d, k) = C(d+k-1, k-1) valid fillings

    // For each position p (0 to m-1) of the special digit:
    // Value contribution = d * 10^p * f(d, k) + (other digits contrib)

    // Other digits contribution by symmetry:
    // Sum of all digits in all fillings = d * f(d, k) (since each filling sums to d)
    // By symmetry each of the k positions gets d/k of the total
    // Wait - we need the weighted positional sum

    // Let's think differently. Fix special digit d at position p.
    // The other k = m-1 positions are at positions {0,...,m-1} \ {p}.
    // Call these positions q_1 < q_2 < ... < q_k.
    // For each valid filling (a_1,...,a_k) with sum d, the value is:
    //   d * 10^p + sum_i a_i * 10^{q_i}
    // Summing over all fillings:
    //   f(d,k) * d * 10^p + sum_i (10^{q_i} * sum over fillings of a_i)
    // By symmetry, sum over fillings of a_i = sum over fillings of a_j for all i,j
    // (since the a_i are interchangeable)
    // Total sum of all a_i over all fillings = d * f(d,k)
    // So sum of a_i for any single position = d * f(d,k) / k

    // Total contribution for special digit d at position p:
    //   f(d,k) * d * 10^p + (d * f(d,k) / k) * sum_{q != p} 10^q

    // But d*f(d,k)/k might not be integer! Actually it is:
    // f(d,k) = C(d+k-1, k-1), and d*C(d+k-1,k-1)/k = d*(d+k-1)!/(k!(d-1)!*k)
    // Hmm, might not always be integer.

    // Alternative: compute separately.
    // avg_digit_sum_per_position(d, k) = d * f(d,k) / k
    // = d * C(d+k-1, k-1) / k = C(d+k-1, k-1) * d / k
    // = (d+k-1)! / ((k-1)! * d!) * d / k * ... let me just compute it directly.
    // d/k * C(d+k-1, k-1) = d/k * (d+k-1)! / ((k-1)! * d!)
    //                     = (d+k-1)! / ((k-1)! * (d-1)! * k)
    //                     = C(d+k-1, k) * ... hmm
    // Actually: d * C(d+k-1, k-1) / k = C(d+k-1, k-1) * d / k
    // Note that C(d+k-1, k-1) * d / k = C(d+k-1, d) * d / k
    // Let's verify: d=3, k=2: C(4,1)*3/2 = 4*3/2 = 6.
    // Compositions of 3 into 2 digits: (0,3),(1,2),(2,1),(3,0) -> digit sums per pos: 0+1+2+3=6, 3+2+1+0=6. Each = 6. Correct!

    // So h(d,k) = d * f(d,k) / k is always an integer? Let's check d=2,k=3:
    // C(4,2)*2/3 = 6*2/3 = 4. Compositions of 2 into 3: (0,0,2),(0,2,0),(2,0,0),(0,1,1),(1,0,1),(1,1,0)
    // Sum per position: 0+0+2+0+1+1=4, 0+2+0+1+0+1=4, 2+0+0+1+1+0=4. Yes, = 4.

    // Actually d*C(d+k-1,k-1)/k = C(d+k-1,k) * d * k! / (k * (k-1)! * ...)
    // Simpler: d*C(d+k-1,k-1)/k = (d+k-1)! / ((k)! * (d-1)!) * d / ...
    // Just note: C(d+k-1,k-1) * d/k = C(d+k-1,d) * d/k = C(d+k-1,d-1) * ...
    // = C(d+k-1, d-1) / 1 * ... NO. Let me just compute:
    // C(n,k-1) * d/k where n=d+k-1:
    // = n! / ((k-1)!(n-k+1)!) * d/k = n! / ((k-1)!(d)!) * d/k
    // = n! * d / (k! * d!) = n! / (k! * (d-1)!) = C(n, k) * ... hmm
    // = C(d+k-1, k) * 1 ... let me just verify once more:
    // C(d+k-1, k) = (d+k-1)! / (k! * (d-1)!)
    // d * C(d+k-1, k-1) / k = d * (d+k-1)! / ((k-1)! * d!) / k
    //                       = (d+k-1)! / ((k-1)! * (d-1)!) / k  [since d/d! = 1/(d-1)!]
    //                       = (d+k-1)! / (k! * (d-1)!)
    //                       = C(d+k-1, k)
    // So h(d,k) = C(d+k-1, k).

    // Now we also need to handle the leading zero constraint.
    // For an m-digit number, position m-1 is the leading digit.
    // Case 1: special digit is at position m-1 (leading). Then d >= 1 (guaranteed).
    //   Other k = m-1 digits at positions 0..m-2. No leading zero issue.
    //   Contribution = f(d,k) * d * 10^(m-1) + h(d,k) * sum_{q=0}^{m-2} 10^q
    //                = f(d,k) * d * 10^(m-1) + h(d,k) * geo_sum(m-1)
    //
    // Case 2: special digit at position p < m-1.
    //   Leading digit (position m-1) must be >= 1.
    //   Total (no constraint) minus (leading digit = 0) cases.
    //
    //   No constraint: same formula as above
    //   Leading digit = 0 constraint: digit at pos m-1 is 0, remaining k-1 other digits sum to d
    //   With f2 = f(d, k-1) fillings. But we also need the contributions.
    //
    //   Effectively, for each p < m-1:
    //   Valid contribution = (unconstrained) - (leading zero contribution)

    // Let me reorganize.
    // For m-digit numbers with special digit d:
    // Let k = m - 1 (number of other digits)

    // Sum over all positions p of special digit:
    // A) p = m-1 (special digit leads):
    //    contrib_A = d * 10^(m-1) * f(d,k) + C(d+k-1,k) * geo_sum(m-1)
    //
    // B) p in {0, ..., m-2} (special digit not leading):
    //    For each p, unconstrained:
    //      contrib_p = d * 10^p * f(d,k) + C(d+k-1,k) * (geo_sum(m) - 10^p)
    //    Wait, sum_{q=0,q!=p}^{m-1} 10^q = geo_sum(m) * 9 ... no.
    //    Let S_all = sum_{q=0}^{m-1} 10^q = (10^m - 1)/9
    //    sum_{q!=p} 10^q = S_all - 10^p
    //
    //    Unconstrained contrib_p = d * 10^p * f(d,k) + h(d,k) * (S_all - 10^p)
    //
    //    Leading zero correction (digit at pos m-1 is 0):
    //    The k-1 remaining other digits sum to d, with f(d,k-1) fillings.
    //    h2 = C(d+k-2, k-1) (by same formula with k-1)
    //    correction_p = d * 10^p * f(d,k-1) + h2 * (sum_{q!=p,q!=m-1} 10^q)
    //                 = d * 10^p * f(d,k-1) + h2 * (S_all - 10^p - 10^(m-1))
    //
    //    Net contrib for position p: contrib_p - correction_p

    // Summing over all p in {0,...,m-2}:
    // Sum of 10^p for p=0..m-2 = geo_sum(m-1) = (10^(m-1)-1)/9
    // Sum of d*10^p = d * geo_sum(m-1)
    // Number of positions = m-1 = k

    // Unconstrained sum over p=0..m-2:
    //   d * f(d,k) * geo_sum(m-1) + h(d,k) * (k * S_all - geo_sum(m-1))
    //   = d * f(d,k) * G1 + h(d,k) * (k*S_all - G1)
    //   where G1 = geo_sum(m-1)

    // Leading zero correction sum over p=0..m-2:
    //   d * f(d,k-1) * G1 + h2 * (k * S_all - G1 - k * 10^(m-1))
    //   = d * f(d,k-1) * G1 + h2 * (k * (S_all - 10^(m-1)) - G1)
    //   = d * f(d,k-1) * G1 + h2 * (k * geo_sum(m-1) - G1)
    //   Hmm wait: sum_{q!=p,q!=m-1} 10^q = S_all - 10^p - 10^(m-1)
    //   Summing over p=0..m-2: sum_p (S_all - 10^p - 10^(m-1)) = k*S_all - G1 - k*10^(m-1)

    // This is getting complex but correct. Let me implement step by step.

    // Precompute powers of 10 mod MOD
    vector<ll> pow10(N + 1);
    pow10[0] = 1;
    for (int i = 1; i <= N; i++) {
        pow10[i] = (lll)pow10[i-1] * 10 % MOD;
    }

    // geo_sum(n) = (10^n - 1) / 9 mod MOD
    // For n >= 16, 10^n mod MOD = 0, so geo_sum(n) = -inv9 mod MOD = (MOD - inv9) % MOD
    auto geo = [&](int n) -> ll {
        if (n <= 0) return 0;
        ll pn = pow10[n];
        return (lll)(pn - 1 + MOD) % MOD * inv9 % MOD;
    };

    for (int m = 2; m <= N; m++) {
        int k = m - 1; // number of non-special digits

        for (int d = 1; d <= 9; d++) {
            ll fdk = f(d, k);          // C(d+k-1, k-1)
            ll hdk = comb((ll)d+k-1, k); // C(d+k-1, k)
            ll fdk1 = (k >= 2) ? f(d, k-1) : 0; // f(d, k-1)
            ll hdk1 = (k >= 2) ? comb((ll)d+k-2, k-1) : 0;

            ll S_all = geo(m);   // sum 10^q for q=0..m-1
            ll G1 = geo(m-1);    // sum 10^q for q=0..m-2
            ll pm1 = pow10[m-1]; // 10^(m-1)

            // Case A: special digit at leading position (p = m-1)
            {
                ll cA = ((lll)d % MOD * pm1 % MOD * fdk % MOD +
                         (lll)hdk * G1 % MOD) % MOD;
                answer = (answer + cA) % MOD;
            }

            // Case B: special digit at positions p = 0..m-2
            // Unconstrained sum:
            //   d * fdk * G1 + hdk * (k * S_all - G1)
            ll unconstrained_B = ((lll)d % MOD * fdk % MOD * G1 % MOD +
                                  (lll)hdk % MOD * ((lll)k * S_all % MOD - G1 + MOD) % MOD) % MOD;

            // Leading zero correction sum:
            if (k >= 2) {
                // k * (S_all - 10^(m-1)) - G1 = k * G1 - G1 = (k-1) * G1
                // Wait: S_all - 10^(m-1) = geo(m) - 10^(m-1) = sum_{q=0}^{m-2} 10^q = G1
                // So: k * G1 - G1 = (k-1) * G1
                ll correction_B = ((lll)d % MOD * fdk1 % MOD * G1 % MOD +
                                   (lll)hdk1 % MOD * ((lll)(k-1) % MOD * G1 % MOD) % MOD) % MOD;
                ll net_B = (unconstrained_B - correction_B % MOD + MOD) % MOD;
                answer = (answer + net_B) % MOD;
            } else {
                // k = 1, m = 2: only one other digit, it must equal d
                // If special at pos 0: number = d*10 + d (but leading is the other digit = d, which is fine since d>=1... wait)
                // Actually k=1: there's 1 other digit summing to d, so other digit = d
                // Positions: p=0, other digit at pos 1 (leading)
                // Value = d + d*10 = 11*d, leading digit = d >= 1, always valid
                // Also p=1 handled in case A: value = d*10 + d = 11*d
                // But case A already counted p=m-1=1.
                // For case B, p=0: other digit at pos 1 must be d (>=1, ok)
                // unconstrained_B covers this correctly with no correction needed
                answer = (answer + unconstrained_B) % MOD;
            }
        }
    }

    // Add 1-digit numbers that are DS-numbers: none (need at least 2 digits for splitting)
    // Actually, a single digit d: the sum of "all other digits" is 0, so d should equal 0.
    // Only 0 would qualify, but 0 doesn't contribute to sum. So nothing to add.

    cout << answer << endl;

    return 0;
}
