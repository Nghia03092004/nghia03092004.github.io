"""
Problem 951: Euler Product Approximation

Compute floor(E(3, 10^6) * 10^15) where E(3, N) = prod_{p <= N} (1 - p^{-3})^{-1}.

The Euler product for zeta(s) converges to Apery's constant zeta(3) = 1.2020569...
We accumulate the product in log-space for numerical stability:
    L = sum_{p <= N} -ln(1 - p^{-3})
    E = exp(L)

Key identity: zeta(s) = prod_p (1 - p^{-s})^{-1}  (Re(s) > 1)
Error bound: zeta(3) - E(3,N) ~ 4*zeta(3) / (N^2 * ln(N))
"""

from math import log, exp, isqrt

ZETA3 = 1.2020569031595942854169

def sieve_of_eratosthenes(n: int) -> list[int]:
    """Return all primes up to n using the Sieve of Eratosthenes."""
    is_prime = bytearray(b'\x01') * (n + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, isqrt(n) + 1):
        if is_prime[i]:
            is_prime[i * i::i] = bytearray(len(is_prime[i * i::i]))
    return [i for i in range(2, n + 1) if is_prime[i]]

def euler_product(s: float, N: int) -> tuple[float, list[tuple[int, float]]]:
    """Compute E(s, N) = prod_{p<=N} (1 - p^{-s})^{-1} in log-space.

    Returns:
        (E, checkpoints): the product value and a list of (prime, running_product)
    """
    primes = sieve_of_eratosthenes(N)
    log_prod = 0.0
    checkpoints = []
    for p in primes:
        log_prod += -log(1 - p ** (-s))
        if p <= 500 or p % 5000 < 10:
            checkpoints.append((p, exp(log_prod)))
    return exp(log_prod), checkpoints

def solve(N: int = 10**6) -> int:
    """Compute floor(E(3, 10^6) * 10^15)."""
    E, _ = euler_product(3.0, N)
    return int(E * 10**15)

# --- Compute answer ---
N = 10**6
E_full, checkpoints = euler_product(3.0, N)
answer = int(E_full * 10**15)

assert abs(E_full - ZETA3) < 1e-12, f"E(3,10^6) = {E_full}, expected ~{ZETA3}"
print(answer)
