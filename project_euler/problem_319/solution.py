"""
Problem 319: Bounded Sequences

t(n) = number of sequences (x_1, ..., x_n) where x_i = floor(alpha^i)
for some alpha in [2, 3).

Formula: t(n) = sum_{k=1}^{n} mu(k) * g(floor(n/k))
where g(m) = G(3,m) - G(2,m), G(v,m) = v * (v^m - 1) / (v - 1)

Uses Mobius function prefix sums (Mertens function) and matrix exponentiation.

Answer: 268457129
"""

import sys
sys.setrecursionlimit(200000)

MOD = 10**9
N = 10**10
LMT = 10**7

def solve():
    # Sieve Mobius function up to LMT
    mu_val = [0] * (LMT + 1)
    mu_val[1] = 1
    is_composite = [False] * (LMT + 1)
    primes = []

    for i in range(2, LMT + 1):
        if not is_composite[i]:
            primes.append(i)
            mu_val[i] = -1
        for p in primes:
            if i * p > LMT:
                break
            is_composite[i * p] = True
            if i % p == 0:
                mu_val[i * p] = 0
                break
            else:
                mu_val[i * p] = -mu_val[i]

    # Prefix sums of Mobius function mod MOD
    mu_prefix = [0] * (LMT + 1)
    for i in range(1, LMT + 1):
        mu_prefix[i] = (mu_prefix[i - 1] + mu_val[i]) % MOD

    # Memoized Mertens function for large values
    memo = {}

    def mertens(n):
        if n <= LMT:
            return mu_prefix[n]
        if n in memo:
            return memo[n]
        ret = 1  # M(1) = 1
        i = 2
        while i <= n:
            j = n // (n // i) + 1
            ret = (ret + MOD - (j - i) % MOD * mertens(n // i) % MOD) % MOD
            i = j
        memo[n] = ret
        return ret

    # Matrix exponentiation for G(v, x) = v * (v^x - 1) / (v - 1) mod MOD
    def mat_mul(A, B):
        return [
            [(A[0][0] * B[0][0] + A[0][1] * B[1][0]) % MOD,
             (A[0][0] * B[0][1] + A[0][1] * B[1][1]) % MOD],
            [(A[1][0] * B[0][0] + A[1][1] * B[1][0]) % MOD,
             (A[1][0] * B[0][1] + A[1][1] * B[1][1]) % MOD]
        ]

    def G(v, x):
        if x == 0:
            return 0
        A = [[v, 1], [0, 1]]
        B = [[1, 0], [0, 1]]  # identity
        e = x
        while e > 0:
            if e & 1:
                B = mat_mul(B, A)
            A = mat_mul(A, A)
            e >>= 1
        return v * B[0][1] % MOD

    def g(x):
        return (G(3, x) - G(2, x) + MOD) % MOD

    # Compute t(N) = sum_{k=1}^{N} mu(k) * g(floor(N/k))
    ans = 0
    prev_m = 0
    i = 1
    while i <= N:
        j = N // (N // i) + 1
        cur_m = mertens(j - 1)
        block_sum = (cur_m - prev_m + MOD) % MOD
        ans = (ans + block_sum * g(N // i)) % MOD
        prev_m = cur_m
        i = j

    print(ans)

if __name__ == "__main__":
    solve()
