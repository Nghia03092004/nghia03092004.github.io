"""
Problem 342: The Totient of a Square Is a Cube

Find the sum of all n with 1 < n < 10^10 such that phi(n^2) is a perfect cube.

phi(n^2) = n * phi(n) = prod p_i^(2*a_i - 1) * (p_i - 1)

For this to be a perfect cube, every prime exponent in the product must be
divisible by 3.

We use DFS over prime factorizations of n, tracking exponent residues mod 3.
"""

def solve():
    LIMIT = 10**10

    # Sieve primes up to 200000 (sufficient since smallest prime factor
    # squared must be < LIMIT, and we have cross-factor contributions)
    def sieve(lim):
        is_prime = [True] * (lim + 1)
        is_prime[0] = is_prime[1] = False
        for i in range(2, int(lim**0.5) + 1):
            if is_prime[i]:
                for j in range(i*i, lim + 1, i):
                    is_prime[j] = False
        return [i for i in range(2, lim + 1) if is_prime[i]]

    primes = sieve(200000)

    def factorize(x):
        factors = []
        d = 2
        while d * d <= x:
            if x % d == 0:
                e = 0
                while x % d == 0:
                    x //= d
                    e += 1
                factors.append((d, e))
            d += 1
        if x > 1:
            factors.append((x, 1))
        return factors

    total = 0

    def dfs(pidx, n_val, res3):
        nonlocal total
        # Check if all residues are 0 mod 3
        if all(v % 3 == 0 for v in res3.values()) and n_val > 1:
            total += n_val

        for i in range(pidx, len(primes)):
            p = primes[i]
            if n_val * p >= LIMIT:
                break

            pf = factorize(p - 1)
            saved = dict(res3)

            # Add (p-1) contribution (once)
            for q, e in pf:
                res3[q] = (res3.get(q, 0) + e) % 3

            cur = n_val
            for a in range(1, 100):
                cur *= p
                if cur >= LIMIT:
                    break
                # Exponent of p: 2a-1. Delta from previous: +2 (or +1 for a=1)
                if a == 1:
                    res3[p] = (res3.get(p, 0) + 1) % 3
                else:
                    res3[p] = (res3.get(p, 0) + 2) % 3

                dfs(i + 1, cur, res3)

            # Restore state
            res3.clear()
            res3.update(saved)

    dfs(0, 1, {})
    print(total)

if __name__ == "__main__":
    solve()
