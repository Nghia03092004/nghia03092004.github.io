"""
Problem 180: Rational Zeros of a Function of Three Variables

Find S(35): sum of all distinct values s = x+y+z from golden triples of order 35.
x, y, z are rationals a/b with 1 <= a < b <= k (proper fractions, strictly less than 1).

The relevant equations (from FLT, only |m| <= 2 have solutions):
  m=1:  x + y = z
  m=2:  x^2 + y^2 = z^2
  m=-1: 1/x + 1/y = 1/z => z = xy/(x+y)
  m=-2: 1/x^2 + 1/y^2 = 1/z^2 => z = xy/sqrt(x^2+y^2)

Express S(35) as p/q in lowest terms, output p+q.
"""

from fractions import Fraction
from math import isqrt

K = 35

# Generate all valid rationals a/b with 1 <= a < b <= K (proper fractions)
rat_set = set()
for b in range(2, K + 1):
    for a in range(1, b):
        rat_set.add(Fraction(a, b))

rat_list = sorted(rat_set)

def is_valid(z):
    """Check if z is a valid proper fraction a/b with 1 <= a < b <= K."""
    if z <= 0 or z >= 1:
        return False
    return z.denominator <= K

s_values = set()

for x in rat_list:
    for y in rat_list:
        # Case m=1: x + y = z
        z = x + y
        if is_valid(z):
            s_values.add(x + y + z)

        # Case m=-1: z = xy/(x+y)
        z = (x * y) / (x + y)
        if is_valid(z):
            s_values.add(x + y + z)

        # Case m=2: z = sqrt(x^2 + y^2)
        z_sq = x * x + y * y
        num = z_sq.numerator
        den = z_sq.denominator
        sn = isqrt(num)
        sd = isqrt(den)
        if sn * sn == num and sd * sd == den:
            z = Fraction(sn, sd)
            if is_valid(z):
                s_values.add(x + y + z)

            # Case m=-2: z = xy/sqrt(x^2+y^2)
            sqrt_sum = Fraction(sn, sd)
            if sqrt_sum > 0:
                z2 = (x * y) / sqrt_sum
                if is_valid(z2):
                    s_values.add(x + y + z2)

total = sum(s_values)
p = total.numerator
q = total.denominator
print(p + q)
