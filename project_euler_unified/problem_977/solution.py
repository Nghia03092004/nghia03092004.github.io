"""
Problem 977: Dedekind Sum Computation

Compute the weighted sum of Dedekind sums:
    S = sum over k=2..100, h=1..k-1 with gcd(h,k)=1 of 12*k*s(h,k)

where s(h,k) = sum_{r=1}^{k-1} ((r/k)) * ((hr/k))
and ((x)) is the sawtooth function: x - floor(x) - 1/2 for non-integer x, 0 otherwise.

Key insight: s(h,k) + s(k-h,k) = 0 for all valid h,k with gcd(h,k)=1.
Since pairing h with k-h covers all coprime residues, the total sum is 0.

Answer: 0

Methods:
    - sawtooth(x): Sawtooth function ((x)) using exact Fraction arithmetic
    - dedekind_sum(h, k): Direct computation of s(h,k)
    - dedekind_reciprocity_check(h, k): Verify reciprocity law
    - verify_pairing_symmetry(k_max): Verify s(h,k) = -s(k-h,k) for small k
"""

from math import gcd
from fractions import Fraction


def sawtooth(x):
    """((x)) = x - floor(x) - 1/2 for non-integer x, 0 for integer x."""
    if x == int(x):
        return Fraction(0)
    return x - int(x) - Fraction(1, 2)

def dedekind_sum(h, k):
    """Compute s(h,k) = sum_{r=1}^{k-1} ((r/k)) * ((hr/k))."""
    s = Fraction(0)
    for r in range(1, k):
        s += sawtooth(Fraction(r, k)) * sawtooth(Fraction(h * r, k))
    return s


def dedekind_reciprocity_check(h, k):
    """
    Verify the Dedekind reciprocity law:
    s(h,k) + s(k,h) = (h/k + k/h + 1/(hk))/12 - 1/4
    for gcd(h,k) = 1.
    """
    lhs = dedekind_sum(h, k) + dedekind_sum(k, h)
    rhs = (Fraction(h, k) + Fraction(k, h) + Fraction(1, h * k)) / 12 - Fraction(1, 4)
    return lhs == rhs


def verify_pairing_symmetry(k_max=30):
    """Verify that s(h,k) + s(k-h,k) = 0 for all coprime h < k."""
    for k in range(2, k_max + 1):
        for h in range(1, k):
            if gcd(h, k) == 1:
                s1 = dedekind_sum(h, k)
                s2 = dedekind_sum(k - h, k)
                assert s1 + s2 == 0, f"Symmetry failed for h={h}, k={k}"
    return True

# Verification

# Known small values
assert dedekind_sum(1, 2) == Fraction(0)
assert dedekind_sum(1, 3) == Fraction(1, 18)
assert dedekind_sum(1, 5) == Fraction(1, 5)

# Reciprocity check
assert dedekind_reciprocity_check(1, 5)
assert dedekind_reciprocity_check(2, 7)
assert dedekind_reciprocity_check(3, 11)

# Pairing symmetry
verify_pairing_symmetry(20)

# By the pairing argument s(h,k) = -s(k-h,k), the full sum is 0
answer = 0
print(answer)
