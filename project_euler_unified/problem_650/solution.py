#!/usr/bin/env python3
"""Project Euler Problem 650: Divisors of Binomial Product"""

MOD = 1000000007

def solve():
    N = 20000

    # Sieve of smallest prime factors
    spf = list(range(N + 1))
    for i in range(2, int(N**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, N + 1, i):
                if spf[j] == j:
                    spf[j] = i

    # Collect primes
    primes = [i for i in range(2, N + 1) if spf[i] == i]
    prime_idx = {p: i for i, p in enumerate(primes)}
    np_ = len(primes)

    def vp_val(n, p):
        cnt = 0
        while n % p == 0:
            n //= p
            cnt += 1
        return cnt

    def vp_factorial(n, p):
        res = 0
        pk = p
        while pk <= n:
            res += n // pk
            pk *= p
        return res

    # Precompute modular inverses
    inv_pm1 = [pow(p - 1, MOD - 2, MOD) if p > 1 else 0 for p in primes]

    expo = [0] * np_

    S = 1  # D(1) = 1

    for n in range(2, N + 1):
        # Update exponents: delta = (n-1)*v_p(n) - v_p((n-1)!)
        for i in range(np_):
            p = primes[i]
            if p > n:
                break
            vpn = vp_val(n, p)
            delta = (n - 1) * vpn - vp_factorial(n - 1, p)
            expo[i] += delta

        # Compute D(n)
        D = 1
        for i in range(np_):
            p = primes[i]
            if p > n:
                break
            if expo[i] > 0:
                num = (pow(p, expo[i] + 1, MOD) - 1) % MOD
                D = D * num % MOD * inv_pm1[i] % MOD

        S = (S + D) % MOD

    print(S)

solve()
