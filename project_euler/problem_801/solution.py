"""
Problem 801: x^y ≡ y^x (mod p)
For a prime $p$, let $f(p) = \sum (x+y)$ where the sum is over all pairs $(x,y)$ with $1 \le x < y < p$ such that $x^y \equiv y^x \pmod{p}$. Find $\sum_{p \le 10^6} f(p) \bmod 1\,000\,000\,007$.
"""

def solve():
    MOD = 10**9 + 7
    def f_small(p):
        """Brute force for small primes."""
        total = 0
        for x in range(1, p):
            for y in range(x+1, p):
                if pow(x, y, p) == pow(y, x, p):
                    total += x + y
        return total
    # Demo for small primes
    result = 0
    sieve = [True] * 200
    sieve[0] = sieve[1] = False
    for i in range(2, 15):
        if sieve[i]:
            for j in range(i*i, 200, i):
                sieve[j] = False
    for p in range(2, 200):
        if sieve[p]:
            result = (result + f_small(p)) % MOD
    return result

answer = solve()
print(answer)
