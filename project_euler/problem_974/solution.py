"""
Problem 974: Mobius Function Partial Sums

Compute the sum of |M(n)| for n = 1 to 10^5, where M(n) = sum_{k=1}^{n} mu(k)
is the Mertens function.

Key results:
    - mu(n) is the Mobius function: mu(1)=1, mu(n)=0 if n has squared factor,
      mu(n)=(-1)^k if n is product of k distinct primes
    - M(n) = sum of mu(1..n) is the Mertens function
    - The Mertens conjecture |M(n)| < sqrt(n) was disproved but holds for small n
    - answer = sum of |M(n)| for n = 1..10^5

Methods:
    1. linear_sieve_mobius  — compute mu(n) via linear sieve
    2. mertens_function     — prefix sums to get M(n)
    3. mobius_distribution   — count of mu(n) = -1, 0, +1
    4. mertens_vs_sqrt_bound — compare |M(n)| against sqrt(n)
"""

from collections import Counter

def linear_sieve_mobius(N):
    """Compute mu[0..N] using a linear sieve. Returns (mu, primes)."""
    mu = [0] * (N + 1)
    mu[1] = 1
    is_prime = [True] * (N + 1)
    primes = []
    for i in range(2, N + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > N:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]
    return mu, primes

def mertens_function(mu, N):
    """Compute M[0..N] where M[n] = sum mu[1..n]."""
    M = [0] * (N + 1)
    for n in range(1, N + 1):
        M[n] = M[n - 1] + mu[n]
    return M

def mobius_distribution(mu, N):
    """Count occurrences of mu(n) = -1, 0, +1 for n = 1..N."""
    counts = Counter(mu[n] for n in range(1, N + 1))
    return counts

def mertens_vs_sqrt(M, N, step=10):
    """Compute |M(n)| / sqrt(n) at sampled points."""
    ns = list(range(step, N + 1, step))
    ratios = [abs(M[n]) / (n ** 0.5) for n in ns]
    return ns, ratios

#  Verification
# Known: mu(1)=1, mu(2)=-1, mu(3)=-1, mu(4)=0, mu(5)=-1, mu(6)=1
# M(1)=1, M(2)=0, M(3)=-1, M(4)=-1, M(5)=-2, M(6)=-1
_mu, _ = linear_sieve_mobius(10)
assert _mu[1] == 1
assert _mu[2] == -1
assert _mu[3] == -1
assert _mu[4] == 0
assert _mu[5] == -1
assert _mu[6] == 1
_M = mertens_function(_mu, 10)
assert _M[1] == 1
assert _M[6] == -1

#  Main computation
N = 10 ** 5
mu, primes = linear_sieve_mobius(N)
M = mertens_function(mu, N)
answer = sum(abs(M[n]) for n in range(1, N + 1))
print(answer)

#  Visualization — 4-panel figure
