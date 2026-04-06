"""
Problem 239: Twenty-two Foolish Primes

A random permutation of {1, 2, ..., 100} is selected. What is the probability
that exactly 22 of the 25 prime-numbered discs are NOT in their natural position?

This means exactly 3 of the 25 primes ARE fixed points.

Approach:
1. Choose which 3 primes are fixed: C(25, 3) = 2300.
2. The remaining 97 elements fill 97 positions. Among them, 22 are primes
   that must NOT be in their natural positions. 75 are non-primes (unrestricted).
3. By inclusion-exclusion on the 22 forbidden constraints:
   IE = sum_{k=0}^{22} (-1)^k * C(22,k) * (97-k)!
4. Probability = C(25,3) * IE / 100!

The answer is expressed as 0.abcdefghijkl (12 decimal places).
"""

from math import comb, factorial
from fractions import Fraction

# Inclusion-exclusion: 22 primes must avoid their positions among 97 total
ie_sum = Fraction(0)
for k in range(23):  # k = 0, 1, ..., 22
    ie_sum += (-1)**k * Fraction(comb(22, k) * factorial(97 - k))

# Total favorable permutations
favorable = Fraction(comb(25, 3)) * ie_sum

# Probability
prob = favorable / Fraction(factorial(100))

# Format to 12 decimal places
# Convert to high-precision decimal
from decimal import Decimal, getcontext
getcontext().prec = 50

# Compute numerator and denominator
n = prob.numerator
d = prob.denominator
result = Decimal(n) / Decimal(d)

print(f"{result:.12f}")
