"""
Problem 987: Cyclotomic Polynomial Evaluation

The n-th cyclotomic polynomial Phi_n(x) is the minimal polynomial over Q
whose roots are the primitive n-th roots of unity. We compute:

    S = sum_{n=1}^{500} Phi_n(2)  mod (10^9 + 7)

Key identity:  x^n - 1 = prod_{d|n} Phi_d(x)
Mobius form:   Phi_n(x) = prod_{d|n} (x^d - 1)^{mu(n/d)}
Recurrence:    Phi_n(x) = (x^n - 1) / prod_{d|n, d<n} Phi_d(x)
"""

from math import gcd

MOD = 10**9 + 7

# --- Mobius function sieve ---
def mobius_sieve(n: int):
    """Compute mu(k) for k = 0, 1, ..., n via linear sieve."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_prime = [True] * (n + 1)
    primes = []
    for i in range(2, n + 1):
        if is_prime[i]:
            primes.append(i)
            mu[i] = -1  # prime => mu = -1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = False
            if i % p == 0:
                mu[i * p] = 0  # p^2 divides i*p
                break
            else:
                mu[i * p] = -mu[i]
    return mu

def divisors(n: int):
    """Return sorted list of divisors of n."""
    divs = []
    d = 1
    while d * d <= n:
        if n % d == 0:
            divs.append(d)
            if d != n // d:
                divs.append(n // d)
        d += 1
    return sorted(divs)

# --- Method 1: Divisor recurrence ---
def solve_recurrence(N: int, mod: int):
    """Compute Phi_n(2) mod p for n=1..N using the divisor recurrence.

    Phi_n(2) = (2^n - 1) / prod_{d|n, d<n} Phi_d(2)  mod p
    """
    phi = [0] * (N + 1)
    for n in range(1, N + 1):
        num = (pow(2, n, mod) - 1) % mod
        den = 1
        for d in divisors(n):
            if d < n:
                den = den * phi[d] % mod
        phi[n] = num * pow(den, mod - 2, mod) % mod
    return phi, sum(phi[1:]) % mod

# --- Method 2: Mobius product formula ---
def solve_mobius(N: int, mod: int):
    """Compute Phi_n(2) mod p using the Mobius product formula.

    Phi_n(2) = prod_{d|n} (2^d - 1)^{mu(n/d)}  mod p
    """
    mu = mobius_sieve(N)
    pow2m1 = [0] * (N + 1)  # 2^d - 1 mod p
    for d in range(1, N + 1):
        pow2m1[d] = (pow(2, d, mod) - 1) % mod

    phi = [0] * (N + 1)
    for n in range(1, N + 1):
        result = 1
        for d in divisors(n):
            m = mu[n // d]
            if m == 1:
                result = result * pow2m1[d] % mod
            elif m == -1:
                result = result * pow(pow2m1[d], mod - 2, mod) % mod
            # m == 0 contributes factor 1
        phi[n] = result
    return phi, sum(phi[1:]) % mod

# --- Method 3: Exact computation (small n, for verification) ---
def cyclotomic_exact(N: int) -> list:
    """Compute exact Phi_n(2) for n = 1..N (no modular reduction)."""
    phi = [0] * (N + 1)
    for n in range(1, N + 1):
        num = 2**n - 1
        den = 1
        for d in divisors(n):
            if d < n:
                den *= phi[d]
        phi[n] = num // den
    return phi

# --- Compute and verify ---
N = 500

phi_rec, ans_rec = solve_recurrence(N, MOD)
phi_mob, ans_mob = solve_mobius(N, MOD)

assert ans_rec == ans_mob, f"Mismatch: {ans_rec} != {ans_mob}"

# Verify small cases with exact computation
phi_exact = cyclotomic_exact(50)
expected = {1: 1, 2: 3, 3: 7, 4: 5, 5: 31, 6: 3, 7: 127, 8: 17, 10: 11, 12: 13}
for n, val in expected.items():
    assert phi_exact[n] == val, f"Phi_{n}(2) = {phi_exact[n]}, expected {val}"

# Verify fundamental identity: prod_{d|n} Phi_d(2) = 2^n - 1
for n in range(1, 51):
    product = 1
    for d in divisors(n):
        product *= phi_exact[d]
    assert product == 2**n - 1, f"Identity failed for n={n}"

# Verify Mobius formula against exact for small n
for n in range(1, 51):
    assert phi_rec[n] == phi_exact[n] % MOD

print(ans_rec)
