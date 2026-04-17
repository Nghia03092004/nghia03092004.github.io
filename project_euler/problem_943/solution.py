"""
Problem 943: Dirichlet Series Evaluation

Compute D(s) = sum_{n=1}^{N} mu(n) / n^s  for s = 2, which converges to 6/pi^2
(the reciprocal of the Riemann zeta function at s=2).

The Mobius function mu(n) is:
    mu(1) = 1
    mu(n) = (-1)^k if n is a product of k distinct primes
    mu(n) = 0      if n has a squared prime factor

The target is floor(D(2) * 10^12) for large N.

Results:
    - D(2) -> 6/pi^2 = 0.607927101854...
    - Convergence is O(1/N) for partial sums

Methods:
    1. compute_mobius       -- linear sieve for mu(n)
    2. partial_dirichlet    -- partial sums of mu(n)/n^s
    3. mobius_distribution  -- counts of mu(n) = -1, 0, +1
    4. convergence_rate     -- |D_N(2) - 6/pi^2| analysis
"""
import numpy as np

def compute_mobius(N):
    """Compute mu(n) for n=0..N using a linear sieve."""
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
    return mu

def partial_dirichlet(mu, s, N):
    """Compute sum_{n=1}^{N} mu(n) / n^s."""
    return sum(mu[n] / n**s for n in range(1, N + 1))

def mobius_distribution(mu, N):
    """Count how many n in [1,N] have mu(n) = -1, 0, +1."""
    neg = sum(1 for n in range(1, N + 1) if mu[n] == -1)
    zero = sum(1 for n in range(1, N + 1) if mu[n] == 0)
    pos = sum(1 for n in range(1, N + 1) if mu[n] == 1)
    return neg, zero, pos

def convergence_errors(mu, checkpoints):
    """Compute |D_N(2) - 6/pi^2| at each checkpoint N."""
    target = 6.0 / np.pi**2
    errors = []
    s = 0.0
    idx = 0
    for n in range(1, checkpoints[-1] + 1):
        s += mu[n] / n**2
        if idx < len(checkpoints) and n == checkpoints[idx]:
            errors.append(abs(s - target))
            idx += 1
    return errors

# Verification
mu_small = compute_mobius(20)
assert mu_small[1] == 1
assert mu_small[2] == -1   # prime
assert mu_small[3] == -1   # prime
assert mu_small[4] == 0    # 4 = 2^2
assert mu_small[5] == -1   # prime
assert mu_small[6] == 1    # 2*3, two distinct primes
assert mu_small[8] == 0    # 8 = 2^3
assert mu_small[10] == 1   # 2*5

# Computation
N = 100000
mu = compute_mobius(N)
D2 = partial_dirichlet(mu, 2, N)
print(D2)
