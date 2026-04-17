"""
Problem 549: Divisibility of Factorials
Find S(10^8) where s(n) = smallest m such that n | m!
S(n) = sum of s(i) for i = 2 to n

Key insight: s(n) = max(s(p^e)) over all prime power factors p^e of n.
For a prime power p^e, s(p^e) is the smallest m such that the exponent of p in m! >= e.
By Legendre's formula: v_p(m!) = sum(floor(m/p^k) for k=1,2,...).

We use a sieve to find smallest prime factors, then factorize each n.
"""

import sys

def solve():
    N = 10**8

    # Sieve of smallest prime factors
    spf = list(range(N + 1))
    for i in range(2, int(N**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, N + 1, i):
                if spf[j] == j:
                    spf[j] = i

    def factorial_val(m, p):
        """Exponent of p in m!"""
        v = 0
        pk = p
        while pk <= m:
            v += m // pk
            pk *= p
        return v

    def find_s_prime_power(p, e):
        """Smallest m such that p^e | m! (m is a multiple of p)."""
        # Binary search on multiples of p
        lo, hi = 1, e  # in terms of multiplier k where m = k*p
        # upper bound: k = e suffices since val(ep, p) >= e
        while lo < hi:
            mid = (lo + hi) // 2
            if factorial_val(mid * p, p) >= e:
                hi = mid
            else:
                lo = mid + 1
        return lo * p

    total = 0
    for n in range(2, N + 1):
        tmp = n
        result = 0
        while tmp > 1:
            p = spf[tmp]
            e = 0
            while tmp % p == 0:
                tmp //= p
                e += 1
            sp = find_s_prime_power(p, e)
            if sp > result:
                result = sp
        total += result

    print(total)

if __name__ == "__main__":
    solve()
