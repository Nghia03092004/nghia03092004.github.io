#include <bits/stdc++.h>
using namespace std;

/*
 * Project Euler 788: Dominating Numbers
 *
 * A dominating number is a positive integer that has more than half of its
 * digits equal to the same digit. Find D(2022) mod 1000000007.
 *
 * For a number with n digits, digit d (0-9) dominates if it appears > n/2 times.
 * We count numbers where some digit appears more than half the time.
 *
 * For each digit length n and each digit d, count numbers with d appearing
 * k times where k > n/2, using combinatorics.
 *
 * For digit d != 0: Choose k positions for d (C(n,k)), fill remaining n-k
 * positions with any of 9 digits (not d), but subtract leading zeros.
 *
 * Using inclusion-exclusion: since at most one digit can dominate (appear > n/2 times),
 * D_n = sum over d=0..9, sum over k=ceil(n/2)+1..n of C(n,k)*9^(n-k)
 *       minus correction for leading zeros.
 *
 * Actually: for n-digit numbers (with leading digit nonzero),
 * count = (total dominating among all n-digit strings 0..9)
 *       - (those with leading zero that are dominating)
 *
 * For any n-digit string (digits 0-9), a digit d dominates if it appears > n/2 times.
 * Since only one digit can appear > n/2 times, no inclusion-exclusion needed.
 *
 * Count of n-digit strings where digit d appears exactly k times = C(n,k)*9^(n-k)
 * Total dominating n-digit strings = 10 * sum_{k=floor(n/2)+1}^{n} C(n,k)*9^(n-k)
 *
 * For actual n-digit numbers (no leading zero), subtract those with leading zero:
 * n-digit strings starting with 0 where some digit dominates.
 * = strings of length n with first digit 0 and some digit dominates
 * = (dominating (n-1)-digit strings with d=0 appearing k-1 more times in last n-1 positions)
 *
 * Simpler approach:
 * Total n-digit dominating strings (allowing leading zeros) = T(n)
 * = 10 * sum_{k > n/2} C(n,k) * 9^(n-k)
 *
 * n-digit dominating strings starting with 0:
 * If 0 dominates: 0 is first digit, need 0 to appear > n/2 times total
 *   = sum_{k > n/2} C(n-1, k-1) * 9^(n-k)
 * If d != 0 dominates: first digit is 0, d appears > n/2 times in remaining n-1 positions
 *   = 9 * sum_{k > n/2} C(n-1, k) * 8^(n-1-k)  ... no, remaining n-1 positions have one taken by 0
 *
 * Let me think more carefully.
 * For n-digit numbers (n >= 1), the count of dominating numbers with exactly n digits:
 * = (n-digit dominating strings) - (n-digit dominating strings with leading 0)
 * = T(n) - T_0(n)
 * where T_0(n) = number of n-digit strings starting with 0 that are dominating
 *             = number of (n-1)-digit strings that are dominating = T(n-1) if n > 1
 * Wait, that's not right either, because the leading zero changes which digit dominates.
 *
 * Actually: an n-digit string starting with 0 that is dominating is the same as
 * prepending 0 to an (n-1)-digit string. The resulting n-digit string has digit d
 * dominating if d appears > n/2 times in all n digits.
 * This is NOT the same as T(n-1), because domination threshold changes.
 *
 * Better: direct computation.
 * D(N) = sum_{n=1}^{N} sum_{d=0}^{9} sum_{k=floor(n/2)+1}^{n} f(n, d, k)
 * where f(n, d, k) = number of n-digit numbers (no leading zero) with digit d appearing exactly k times.
 *
 * f(n, d, k) for d != 0:
 *   Choose k positions out of n for d: C(n, k)
 *   Fill remaining n-k positions with digits 0-9 except d: 9^(n-k)
 *   Subtract those with leading zero: first position is NOT one of the k positions of d,
 *     and first position is 0.
 *   = C(n,k)*9^(n-k) - C(n-1,k)*8^(n-1-k)     [... not quite]
 *
 * Wait. Let's be precise.
 * For d != 0:
 *   Total n-digit strings with d appearing exactly k times = C(n,k) * 9^(n-k)
 *   Among these, those with leading 0:
 *     First digit is 0 (not d), so d appears k times in positions 2..n: C(n-1,k)*9^(n-1-k)
 *     But wait, first digit is specifically 0, not just "not d".
 *     First digit = 0, remaining n-1 digits have d exactly k times: C(n-1,k) * 8^(n-1-k)
 *     (remaining n-1-k non-d positions filled with 0-9 except d and except... no, they can be anything except d)
 *     Actually no: first digit is fixed to 0 (which is not d since d!=0).
 *     Remaining n-1 positions: d appears k times, so choose k of n-1: C(n-1,k).
 *     Other n-1-k positions: any digit except d: 9 choices. But one of those 9 choices is 0, fine.
 *     So leading-zero count = C(n-1,k) * 9^(n-1-k)   ... wait no:
 *     We're saying first position = 0 (determined, not d). Remaining n-1 positions: k positions get d,
 *     the other n-1-k get any of 9 digits (not d). But 0 is one of those 9 choices. OK so:
 *     leading_zero_count = C(n-1, k) * 9^(n-1-k)
 *     Hmm, but 0 is already used in position 1 and these other positions CAN also have 0.
 *     Yes, that's fine. Digits can repeat.
 *
 *   f(n, d, k) = C(n,k)*9^(n-k) - C(n-1,k)*9^(n-1-k)   for d != 0, n >= 2
 *   f(1, d, 1) = 1 for d = 1..9 (single digit d)
 *   f(1, d, 0) = ... not relevant since k > n/2 means k >= 1
 *
 * For d = 0:
 *   Total n-digit strings with 0 appearing exactly k times = C(n,k) * 9^(n-k)
 *   Among these, those with leading 0: first digit is 0, so 0 appears at least once.
 *     0 appears k times total, first digit is 0, so 0 appears k-1 times in remaining n-1 positions.
 *     C(n-1, k-1) * 9^(n-k)
 *   f(n, 0, k) = C(n,k)*9^(n-k) - C(n-1,k-1)*9^(n-k)
 *              = 9^(n-k) * [C(n,k) - C(n-1,k-1)]
 *              = 9^(n-k) * C(n-1, k)
 *   For n=1: f(1, 0, k): only k=1 matters, but the only 1-digit number with 0 is "0" which isn't positive.
 *   f(1, 0, 1) = 9^0 * C(0,1) = 0. Good.
 *
 * So:
 * D(N) = sum_{n=1}^{N} [
 *   sum_{d=1}^{9} sum_{k=floor(n/2)+1}^{n} (C(n,k)*9^(n-k) - C(n-1,k)*9^(n-1-k))
 * + sum_{k=floor(n/2)+1}^{n} 9^(n-k) * C(n-1, k)
 * ]
 *
 * Let's simplify. Let T(n) = sum_{k=floor(n/2)+1}^{n} C(n,k)*9^(n-k).
 * Then total dominating strings of length n = 10 * T(n).
 *
 * For d=1..9: sum = 9 * (T(n) - T'(n)) where T'(n) = sum_{k} C(n-1,k)*9^(n-1-k)
 * For d=0: sum = sum_{k} C(n-1,k)*9^(n-k) = 9 * sum_{k} C(n-1,k)*9^(n-1-k) ... no:
 *   = sum_{k} C(n-1,k) * 9^(n-k) = 9 * sum_{k} C(n-1,k) * 9^(n-1-k)  ... 9^(n-k) = 9*9^(n-1-k)
 *   Hmm, n-k and k both change. Let me just compute directly.
 *
 * For n-digit dominating positive integers:
 * = sum_{d=0}^{9} sum_{k=m+1}^{n} f(n,d,k)   where m = floor(n/2)
 *
 * = sum_{d=1}^{9} sum_{k=m+1}^{n} [C(n,k)*9^(n-k) - C(n-1,k)*9^(n-1-k)]
 * + sum_{k=m+1}^{n} C(n-1,k)*9^(n-k)
 *
 * = 9*sum_{k=m+1}^{n} C(n,k)*9^(n-k) - 9*sum_{k=m+1}^{n} C(n-1,k)*9^(n-1-k)
 * + sum_{k=m+1}^{n} C(n-1,k)*9^(n-k)
 *
 * = 9*T(n) - 9*A(n) + B(n)
 * where A(n) = sum_{k=m+1}^{n} C(n-1,k)*9^(n-1-k)
 *       B(n) = sum_{k=m+1}^{n} C(n-1,k)*9^(n-k) = 9*A(n) ... no:
 *       9^(n-k) vs 9^(n-1-k): 9^(n-k) = 9 * 9^(n-1-k)
 *       So B(n) = 9 * A(n)
 *
 * Wait: B(n) = sum_{k=m+1}^{n} C(n-1,k)*9^(n-k)
 * But C(n-1, k) for k = n: C(n-1, n) = 0.
 * So B(n) = sum_{k=m+1}^{n-1} C(n-1,k)*9^(n-k) = 9 * sum_{k=m+1}^{n-1} C(n-1,k)*9^(n-1-k)
 * A(n) = sum_{k=m+1}^{n} C(n-1,k)*9^(n-1-k) = sum_{k=m+1}^{n-1} C(n-1,k)*9^(n-1-k) + C(n-1,n)*9^(-1)
 *       = sum_{k=m+1}^{n-1} C(n-1,k)*9^(n-1-k)   (since C(n-1,n)=0)
 * So B(n) = 9 * A(n).
 *
 * Therefore: 9*T(n) - 9*A(n) + 9*A(n) = 9*T(n).
 *
 * So the count of n-digit dominating positive integers = 9*T(n)?
 * That seems too clean. Let me verify with n=1:
 * T(1) = sum_{k=1}^{1} C(1,1)*9^0 = 1
 * 9*T(1) = 9. For n=1, dominating 1-digit numbers are 1-9 (9 numbers). Correct!
 *
 * For n=2: m=1, k=2 only. T(2) = C(2,2)*9^0 = 1. 9*T(2) = 9.
 * 2-digit dominating numbers: both digits same, from 11,22,...,99 = 9 numbers. Correct!
 *
 * For n=3: m=1, k=2,3. T(3) = C(3,2)*9 + C(3,3)*1 = 27+1 = 28. 9*28 = 252.
 * Let me verify: 3-digit numbers where some digit appears >= 2 times.
 * Total 3-digit numbers = 900.
 * 3-digit numbers where ALL digits are different: 9*9*8 = 648.
 * So dominating = 900 - 648 = 252. Correct!
 *
 * Wait, but dominating means MORE than half, so for n=3, need > 1.5, so >= 2. Yes.
 *
 * So D(N) = sum_{n=1}^{N} 9*T(n) where T(n) = sum_{k=floor(n/2)+1}^{n} C(n,k)*9^(n-k)
 *
 * Now 10*T(n) = sum over all 10 digits d, count of n-length strings where d appears > n/2 times
 * = total dominating n-length strings (with possible leading zeros)
 *
 * And (10-1)*T(n) = 9*T(n) = dominating n-digit positive integers.
 *
 * Wait, that doesn't seem right in general. Let me re-examine...
 * Actually the argument above showed it algebraically. And verified for n=1,2,3.
 *
 * So we need: D(N) = 9 * sum_{n=1}^{N} T(n)   mod 10^9+7
 * where T(n) = sum_{k=floor(n/2)+1}^{n} C(n,k) * 9^(n-k)
 *
 * For N = 2022, we need modular arithmetic with precomputed factorials.
 */

const long long MOD = 1000000007;

long long power(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1) result = result * base % mod;
        base = base * base % mod;
        exp >>= 1;
    }
    return result;
}

int main() {
    int N = 2022;

    // Precompute factorials and inverse factorials
    vector<long long> fact(N + 1), inv_fact(N + 1);
    fact[0] = 1;
    for (int i = 1; i <= N; i++) fact[i] = fact[i-1] * i % MOD;
    inv_fact[N] = power(fact[N], MOD - 2, MOD);
    for (int i = N - 1; i >= 0; i--) inv_fact[i] = inv_fact[i+1] * (i+1) % MOD;

    auto C = [&](int n, int k) -> long long {
        if (k < 0 || k > n) return 0;
        return fact[n] % MOD * inv_fact[k] % MOD * inv_fact[n-k] % MOD;
    };

    // Precompute powers of 9
    vector<long long> pow9(N + 1);
    pow9[0] = 1;
    for (int i = 1; i <= N; i++) pow9[i] = pow9[i-1] * 9 % MOD;

    long long ans = 0;
    for (int n = 1; n <= N; n++) {
        int m = n / 2; // floor(n/2)
        long long T = 0;
        for (int k = m + 1; k <= n; k++) {
            T = (T + C(n, k) % MOD * pow9[n - k]) % MOD;
        }
        ans = (ans + T) % MOD;
    }
    ans = ans * 9 % MOD;

    cout << ans << endl;

    return 0;
}
