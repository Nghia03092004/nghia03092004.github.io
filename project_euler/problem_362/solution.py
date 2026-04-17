"""
Problem 362: Squarefree Factors

f(n) = largest squarefree divisor of n.
Compute sum of f(n) for n = 1 to 10^14.

Approach:
- Write n = a^2 * b where b is squarefree, then f(n) = b
- Sum becomes: sum over a of (sum of squarefree b <= N/a^2)
- Use Mobius function for squarefree sum computation

Answer: 457895958010
"""

import math

def compute_mobius(limit):
    """Compute Mobius function via sieve up to limit."""
    mu = [0] * (limit + 1)
    mu[1] = 1
    is_prime = [True] * (limit + 1)
    primes = []

    for i in range(2, limit + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > limit:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]

    return mu

def T(m):
    """Triangular number: m*(m+1)//2."""
    return m * (m + 1) // 2

def squarefree_sum(x, mu):
    """Sum of all squarefree numbers from 1 to x."""
    sq = int(math.isqrt(x))
    result = 0
    for d in range(1, sq + 1):
        if mu[d] == 0:
            continue
        q = x // (d * d)
        result += mu[d] * d * d * T(q)
    return result

def solve():
    N = 10**14
    sqrtN = int(math.isqrt(N))

    # Need mobius values up to sqrt(N) ~ 10^7
    mu = compute_mobius(sqrtN + 1)

    answer = 0
    for a in range(1, sqrtN + 1):
        limit = N // (a * a)
        if limit == 0:
            break
        answer += squarefree_sum(limit, mu)

    return answer

if __name__ == "__main__":
    answer = solve()
assert answer == 457895958010
print(answer)
    # Expected: 457895958010
