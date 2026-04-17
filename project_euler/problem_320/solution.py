"""
Problem 320: Factorials Divisible by a Huge Integer

N(i) = smallest n such that (i!)^E | n!, where E = 1234567890.
S(u) = sum of N(i) for i = 10 to u.
Find S(1000000) mod 10^18.

Uses incremental tracking of prime targets with binary search,
exploiting the monotonicity of N(i).

Answer: 278157919195482643
"""

import sys

def solve():
    E = 1234567890
    MOD = 10**18
    MAXN = 1000001

    # Smallest prime factor sieve
    spf = list(range(MAXN))
    for i in range(2, int(MAXN**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i*i, MAXN, i):
                if spf[j] == j:
                    spf[j] = i

    # Collect primes
    primes = [i for i in range(2, MAXN) if spf[i] == i]
    prime_idx = {p: idx for idx, p in enumerate(primes)}

    def legendre(n, p):
        """v_p(n!) = sum floor(n/p^k)"""
        s = 0
        pk = p
        while pk <= n:
            s += n // pk
            if pk > n // p:
                break
            pk *= p
        return s

    def min_n_for_target(p, target):
        """Smallest n with v_p(n!) >= target."""
        if target <= 0:
            return 0
        lo, hi = 0, target * p
        while lo < hi:
            mid = (lo + hi) // 2
            if legendre(mid, p) >= target:
                hi = mid
            else:
                lo = mid + 1
        return lo

    num_primes = len(primes)
    target_arr = [0] * num_primes
    np_arr = [0] * num_primes
    current_max = 0
    S = 0

    for i in range(2, MAXN):
        # Factorize i and update prime factor targets
        temp = i
        while temp > 1:
            p = spf[temp]
            idx = prime_idx[p]
            vp = 0
            while temp % p == 0:
                temp //= p
                vp += 1
            target_arr[idx] += E * vp
            np_arr[idx] = min_n_for_target(p, target_arr[idx])
            if np_arr[idx] > current_max:
                current_max = np_arr[idx]

        if i >= 10:
            S = (S + current_max) % MOD

    print(S)

if __name__ == "__main__":
    solve()
