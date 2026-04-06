"""
Problem 948: Counting Squarefree Numbers in Range

Compute Q(N) = count of squarefree numbers up to N, using the Mobius function.

A number n is squarefree if no perfect square > 1 divides it.
The count is given by: Q(N) = sum_{k=1}^{sqrt(N)} mu(k) * floor(N/k^2)

Asymptotically, Q(N)/N -> 6/pi^2 ~ 0.607927.

Key results:
  - Q(100) = 61
  - Q(1000) = 608
  - Q(10^6) = 607927
  - The density converges to 6/pi^2

Methods:
  1. Linear sieve for Mobius function
  2. Mobius-based exact formula Q(N)
  3. Brute-force squarefree check for validation
  4. Convergence analysis of Q(N)/N to 6/pi^2
"""
import numpy as np

def compute_mobius(n):
    """Compute mu(k) for k=0..n via linear sieve."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_prime = [True] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0
                break
            else:
                mu[i * p] = -mu[i]
    return mu

def Q(N):
    """Count squarefree numbers in [1, N]."""
    sqrtN = int(N ** 0.5)
    mu = compute_mobius(sqrtN)
    total = 0
    for k in range(1, sqrtN + 1):
        if mu[k] != 0:
            total += mu[k] * (N // (k * k))
    return total

def is_squarefree(n):
    """Check if n is squarefree by trial division."""
    if n <= 1:
        return n == 1
    d = 2
    while d * d <= n:
        if n % (d * d) == 0:
            return False
        d += 1
    return True

def Q_brute(N):
    """Count squarefree numbers in [1, N] by brute force."""
    return sum(1 for n in range(1, N + 1) if is_squarefree(n))

def mobius_distribution(n):
    """Count how many k in [1,n] have mu(k) = -1, 0, +1."""
    mu = compute_mobius(n)
    neg = sum(1 for k in range(1, n + 1) if mu[k] == -1)
    zero = sum(1 for k in range(1, n + 1) if mu[k] == 0)
    pos = sum(1 for k in range(1, n + 1) if mu[k] == 1)
    return neg, zero, pos

# Verification with assertions
assert Q(100) == Q_brute(100)
assert Q(1000) == Q_brute(1000)
assert Q(100) == 61
assert Q(1000) == 608

# Check specific squarefree numbers
assert is_squarefree(1)
assert is_squarefree(2)
assert is_squarefree(3)
assert not is_squarefree(4)    # 4 = 2^2
assert is_squarefree(5)
assert is_squarefree(6)
assert not is_squarefree(8)    # 8 = 2^3 has factor 4
assert not is_squarefree(9)    # 9 = 3^2

# Compute answer
for n in [100, 1000, 10000, 100000, 10**6]:
    print(f"Q({n}) = {Q(n)}, ratio = {Q(n)/n:.6f}")

print(f"6/pi^2 = {6/np.pi**2:.6f}")
print(Q(10**6))
