"""
Problem 978: Bernoulli Number Modular Sum

Compute sum_{n=0}^{200} (|p_n| + q_n) mod (10^9 + 7),
where B_n = p_n / q_n is the n-th Bernoulli number in lowest terms.

Bernoulli numbers are defined by the exponential generating function:
    t / (e^t - 1) = sum B_n * t^n / n!

Key properties:
    - B_0 = 1, B_1 = -1/2, B_n = 0 for odd n >= 3
    - |B_{2n}| grows roughly as 2 * (2n)! / (2*pi)^{2n}
    - Denominators follow the Von Staudt-Clausen theorem:
      denom(B_{2n}) = product of primes p where (p-1) | 2n

Answer: computed via exact Fraction arithmetic

Methods:
    - compute_bernoulli(n_max): Compute B_0..B_{n_max} via recurrence
    - von_staudt_clausen_denominator(n): Predict denominator via theorem
    - verify_small_values(): Check known Bernoulli numbers
"""

from fractions import Fraction
from math import comb, log10

MOD = 10**9 + 7


def compute_bernoulli(n_max):
    """Compute B_0 to B_{n_max} using the standard recurrence."""
    B = [Fraction(0)] * (n_max + 1)
    B[0] = Fraction(1)
    for n in range(1, n_max + 1):
        B[n] = -sum(Fraction(comb(n + 1, k)) * B[k] for k in range(n)) / (n + 1)
    return B


def von_staudt_clausen_denominator(n):
    """
    For even n >= 2, denom(B_n) = product of primes p where (p-1) | n.
    """
    if n == 0:
        return 1
    if n == 1:
        return 2
    if n % 2 == 1:
        return 1  # B_n = 0 for odd n >= 3
    # Find all primes p where (p-1) divides n
    result = 1
    # Check primes up to n+1
    for p in range(2, n + 2):
        # Simple primality check
        if p < 2:
            continue
        is_prime = True
        for d in range(2, int(p**0.5) + 1):
            if p % d == 0:
                is_prime = False
                break
        if is_prime and n % (p - 1) == 0:
            result *= p
    return result


def verify_small_values(B):
    """Check known Bernoulli numbers."""
    assert B[0] == Fraction(1)
    assert B[1] == Fraction(-1, 2)
    assert B[2] == Fraction(1, 6)
    assert B[4] == Fraction(-1, 30)
    assert B[6] == Fraction(1, 42)
    assert B[8] == Fraction(-1, 30)
    assert B[10] == Fraction(5, 66)
    # Odd Bernoulli numbers (>= 3) are zero
    for n in range(3, 20, 2):
        assert B[n] == 0
    return True

# Computation

B = compute_bernoulli(200)
verify_small_values(B)

# Verify Von Staudt-Clausen for even indices
for n in range(0, 50, 2):
    if n >= 2:
        assert B[n].denominator == von_staudt_clausen_denominator(n), \
            f"VSC failed at n={n}: {B[n].denominator} vs {von_staudt_clausen_denominator(n)}"

answer = 0
for n in range(201):
    p, q = B[n].numerator, B[n].denominator
    answer = (answer + (abs(p) + q) % MOD) % MOD

print(answer)
