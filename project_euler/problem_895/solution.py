"""
Problem 895: Arithmetic Derivative
n' defined by: p'=1 for primes, (ab)' = a'b + ab' (Leibniz rule).
Closed form: n' = n * sum(a_i / p_i) where n = prod(p_i^a_i).
"""

from math import isqrt
from functools import lru_cache

def factorize(n):
    """Return list of (prime, exponent) pairs."""
    factors = []
    d = 2
    while d * d <= n:
        if n % d == 0:
            exp = 0
            while n % d == 0:
                exp += 1
                n //= d
            factors.append((d, exp))
        d += 1
    if n > 1:
        factors.append((n, 1))
    return factors

def arithmetic_derivative(n):
    """Compute n' using the formula n' = n * sum(a_i / p_i)."""
    if n <= 1:
        return 0
    factors = factorize(n)
    # n' = n * sum(a/p) -- compute as integer arithmetic
    # n * sum(a_i/p_i) = sum(a_i * n / p_i)
    result = 0
    for p, a in factors:
        result += a * (n // p)
    return result

@lru_cache(maxsize=None)
def arith_deriv_leibniz(n):
    """Compute n' using the recursive Leibniz definition."""
    if n <= 1:
        return 0
    # Find smallest prime factor
    d = 2
    while d * d <= n:
        if n % d == 0:
            a = d
            b = n // d
            return arith_deriv_leibniz(a) * b + a * arith_deriv_leibniz(b)
        d += 1
    # n is prime
    return 1

def derivative_sieve(N):
    """Compute n' for all n from 0 to N using a sieve."""
    # For each n, accumulate sum(a_i / p_i) as a fraction
    # Actually store n' = n * sum(a_i / p_i)
    deriv = [0] * (N + 1)
    # Use smallest prime factor sieve
    spf = list(range(N + 1))
    for i in range(2, isqrt(N) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, N + 1, i):
                if spf[j] == j:
                    spf[j] = i
    for n in range(2, N + 1):
        tmp = n
        result = 0
        while tmp > 1:
            p = spf[tmp]
            a = 0
            while tmp % p == 0:
                a += 1
                tmp //= p
            result += a * (n // p)
        deriv[n] = result
    return deriv

# --- Verification ---
print("=== Arithmetic Derivative Verification ===")
print(f"{'n':>4} {'Formula':>8} {'Leibniz':>8} {'Match':>6}")
for n in range(0, 31):
    f = arithmetic_derivative(n)
    l = arith_deriv_leibniz(n)
    match = "OK" if f == l else "FAIL"
    print(f"{n:>4} {f:>8} {l:>8} {match:>6}")
    assert f == l

# Sieve verification
print("\n=== Sieve Verification ===")
N = 100
sieve = derivative_sieve(N)
for n in range(2, N + 1):
    assert sieve[n] == arithmetic_derivative(n), f"Mismatch at n={n}"
print(f"Sieve matches formula for all n in [2, {N}]")

# Fixed points: n' = n
print("\n=== Fixed Points (n' = n) ===")
for n in range(2, 10000):
    if arithmetic_derivative(n) == n:
        print(f"  {n} = {factorize(n)}")

# Leibniz rule verification
print("\n=== Leibniz Rule Check ===")
for a, b in [(6, 10), (3, 5), (4, 7), (12, 15)]:
    lhs = arithmetic_derivative(a * b)
    rhs = arithmetic_derivative(a) * b + a * arithmetic_derivative(b)
    print(f"  ({a}*{b})' = {lhs}, {a}'*{b} + {a}*{b}' = {rhs}, Match: {'OK' if lhs == rhs else 'FAIL'}")
    assert lhs == rhs

# Cumulative sums
print("\n=== Cumulative Sum ===")
for N in [10, 20, 50, 100, 500, 1000]:
    s = sum(arithmetic_derivative(n) for n in range(1, N + 1))
    print(f"  sum_{{n=1}}^{{{N}}} n' = {s}")

answer = sum(arithmetic_derivative(n) for n in range(1, 10001))
print(f"\nAnswer: sum_{{n=1}}^{{10000}} n' = {answer}")

# --- 4-Panel Visualization ---
