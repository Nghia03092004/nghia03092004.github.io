"""
Project Euler Problem 686: Powers of Two

Find p(123, 678910): the 678910th smallest j such that 2^j starts with "123".

Key insight: 2^j starts with digits L iff
  log10(L) <= frac(j * log10(2)) < log10(L+1)
where frac(x) is the fractional part of x.

We use the three known gaps between consecutive solutions to speed up the search.
The gaps between consecutive j values where 2^j starts with "123" cycle through
approximately three values: 196, 289, 485.
"""

import math

def solve():
    log10_2 = math.log10(2)
    lo = math.log10(1.23)
    hi = math.log10(1.24)

    target = 678910
    count = 0
    j = 0

    while count < target:
        j += 1
        frac = (j * log10_2) % 1.0
        if lo <= frac < hi:
            count += 1

    print(j)

if __name__ == "__main__":
    solve()
