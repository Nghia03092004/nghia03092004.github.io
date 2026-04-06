"""
Project Euler 446: Retractions B

R(n) is multiplicative with R(p^k) = p^(k-1)*(p+1)
F(N) = sum of R(n^4+4) for n=1..N
n^4 + 4 = (n^2-2n+2)(n^2+2n+2) by Sophie Germain identity
"""

MOD = 1000000007

def factorize(n, primes):
    factors = {}
    for p in primes:
        if p * p > n:
            break
        while n % p == 0:
            factors[p] = factors.get(p, 0) + 1
            n //= p
    if n > 1:
        factors[n] = factors.get(n, 0) + 1
    return factors

def compute_R(factors):
    result = 1
    for p, k in factors.items():
        result = result * (pow(p, k-1, MOD) * ((p + 1) % MOD) % MOD) % MOD
    return result

def solve():
    N = 10**7
    SIEVE_LIMIT = 100100

    # Sieve primes
    is_prime = [True] * (SIEVE_LIMIT + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(SIEVE_LIMIT**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, SIEVE_LIMIT + 1, i):
                is_prime[j] = False
    primes = [i for i in range(2, SIEVE_LIMIT + 1) if is_prime[i]]

    ans = 0
    for n in range(1, N + 1):
        p = n * n - 2 * n + 2
        q = n * n + 2 * n + 2

        fp = factorize(p, primes)
        fq = factorize(q, primes)

        # Merge
        merged = dict(fp)
        for prime, cnt in fq.items():
            merged[prime] = merged.get(prime, 0) + cnt

        r = compute_R(merged)
        ans = (ans + r) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
