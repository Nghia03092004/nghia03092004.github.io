"""
Problem 971: Gaussian Integer Primes

Count Gaussian primes a + bi in the first quadrant with 0 <= a, b <= 500.

A Gaussian integer a + bi is prime if:
    - a > 0, b > 0 and a^2 + b^2 is a rational prime, OR
    - a > 0, b = 0 and a is a rational prime with a ≡ 3 (mod 4), OR
    - a = 0, b > 0 and b is a rational prime with b ≡ 3 (mod 4).

Key results:
    - Interior primes come from norms a^2 + b^2 being prime
    - Axis primes are primes p ≡ 3 (mod 4) placed on either axis
    - answer = total count in [0, 500] x [0, 500]

Methods:
    1. sieve_of_eratosthenes  — standard sieve up to needed bound
    2. count_interior_primes  — pairs (a,b) with a,b > 0 and a^2+b^2 prime
    3. count_axis_primes      — primes p ≡ 3 (mod 4) on the axes
    4. norm_distribution      — distribution of norms a^2+b^2 for Gaussian primes
"""

from math import isqrt
from collections import Counter

def sieve_of_eratosthenes(n):
    """Return bytearray where s[i] is 1 iff i is prime, for 0 <= i <= n."""
    s = bytearray(b'\x01') * (n + 1)
    s[0] = s[1] = 0
    for i in range(2, isqrt(n) + 1):
        if s[i]:
            s[i * i :: i] = bytearray(len(s[i * i :: i]))
    return s

def count_interior_primes(N, is_p):
    """Count pairs (a,b) with 1<=a,b<=N and a^2+b^2 prime. Also collect small ones."""
    count = 0
    small_a, small_b = [], []
    for a in range(1, N + 1):
        for b in range(1, N + 1):
            if is_p[a * a + b * b]:
                count += 1
                if a <= 80 and b <= 80:
                    small_a.append(a)
                    small_b.append(b)
    return count, small_a, small_b

def count_axis_primes(N, is_p):
    """Count Gaussian primes on both axes: (a,0) and (0,b) with a,b ≡ 3 mod 4 and prime."""
    count = 0
    axis_vals = []
    for a in range(1, N + 1):
        if is_p[a] and a % 4 == 3:
            count += 2  # (a, 0) and (0, a)
            axis_vals.append(a)
    return count, axis_vals

def norm_distribution(N, is_p, num_bins=50):
    """Histogram of norms a^2+b^2 for interior Gaussian primes."""
    norms = []
    for a in range(1, N + 1):
        for b in range(1, N + 1):
            n = a * a + b * b
            if is_p[n]:
                norms.append(n)
    return norms

#  Verification
# Small case: Gaussian primes with 0 <= a,b <= 5
_is_p = sieve_of_eratosthenes(50)
_int_count = 0
for a in range(1, 6):
    for b in range(1, 6):
        if _is_p[a * a + b * b]:
            _int_count += 1
_axis_count = 0
for a in range(1, 6):
    if _is_p[a] and a % 4 == 3:
        _axis_count += 2
_total_5 = _int_count + _axis_count
# Interior: (1,1)->2P (1,2)->5P (2,1)->5P (1,4)->17P (4,1)->17P (2,3)->13P (3,2)->13P
# (3,4)->25=5*5 no, (4,3)->25 no, (1,2)->5 yes, etc.
# Axis: 3 is prime and 3%4==3 -> +2
assert _axis_count == 2, f"Expected 2 axis primes for N=5, got {_axis_count}"

#  Main computation
N = 500
M = N * N + N * N  # max possible norm
is_p = sieve_of_eratosthenes(M)

interior_count, gp_a, gp_b = count_interior_primes(N, is_p)
axis_count, axis_vals = count_axis_primes(N, is_p)
answer = interior_count + axis_count
print(answer)

#  Visualization — 4-panel figure
