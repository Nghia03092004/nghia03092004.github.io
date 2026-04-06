"""
Problem 888: Multidimensional Sieve
Mobius inversion and inclusion-exclusion over prime factors.
"""

from math import gcd, isqrt

def mobius_sieve(N):
    """Compute mu(n) for n = 0..N."""
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

def coprime_pairs(N, mu):
    """Count pairs (a,b) in [1,N]^2 with gcd(a,b) = 1."""
    total = 0
    for d in range(1, N + 1):
        total += mu[d] * (N // d) ** 2
    return total

def coprime_pairs_brute(N):
    """Brute force count."""
    count = 0
    for a in range(1, N + 1):
        for b in range(1, N + 1):
            if gcd(a, b) == 1:
                count += 1
    return count

def euler_totient_via_mobius(n, mu):
    """phi(n) = sum_{d|n} mu(d) * (n/d)."""
    total = 0
    for d in range(1, n + 1):
        if n % d == 0:
            total += mu[d] * (n // d)
    return total

# --- Verification ---
N = 100
mu = mobius_sieve(N)

print("=== Mobius Function ===")
for n in range(1, 31):
    print(f"  mu({n:>2}) = {mu[n]:>2}")

print("\n=== Coprime Pairs ===")
for N_test in [1, 2, 3, 5, 10, 20]:
    fast = coprime_pairs(N_test, mu)
    brute = coprime_pairs_brute(N_test)
    print(f"  C_2({N_test:>2}) = {fast:>5} (fast), {brute:>5} (brute), "
          f"{'OK' if fast == brute else 'FAIL'}")
    assert fast == brute

print("\n=== Euler Totient via Mobius ===")
for n in [1, 2, 3, 4, 6, 10, 12, 20, 30]:
    phi = euler_totient_via_mobius(n, mu)
    print(f"  phi({n:>2}) = {phi}")

print("\n=== Verification: sum mu(d) for d|n ===")
for n in [1, 2, 6, 12, 30]:
    s = sum(mu[d] for d in range(1, n + 1) if n % d == 0)
    print(f"  sum mu(d) for d|{n} = {s} (should be {'1' if n == 1 else '0'})")
    assert s == (1 if n == 1 else 0)

answer = coprime_pairs(100, mu)
print(f"\nAnswer: C_2(100) = {answer}")

# --- 4-Panel Visualization ---
