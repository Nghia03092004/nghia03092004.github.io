"""
Problem 143: Investigating the Torricelli Point of a Triangle

At the Torricelli point T, angles ATB = BTC = CTA = 120 degrees.
By cosine rule: a^2 = q^2 + qr + r^2, etc.

We need p, q, r positive integers with p^2+pq+q^2, p^2+pr+r^2, q^2+qr+r^2
all perfect squares, and p+q+r <= 120000.

120-compatible pairs: (u,v) with u^2+uv+v^2 a perfect square.
Primitive parametrization: u = m^2-n^2, v = 2mn+n^2
with m > n >= 1, gcd(m,n) = 1, (m-n) not divisible by 3.
Then u^2+uv+v^2 = (m^2+mn+n^2)^2.
All solutions are k*(u,v) for positive integer k.
"""

from math import gcd, isqrt
from collections import defaultdict

def solve():
    LIMIT = 120000

    # Build adjacency: smaller[u] = set of v < u compatible with u
    smaller = defaultdict(set)

    def add_primitive(a, b):
        """Add primitive pair (a,b) and all its multiples."""
        if a <= 0 or b <= 0:
            return
        u, v = max(a, b), min(a, b)
        k = 1
        while k * (u + v) <= LIMIT:
            smaller[k * u].add(k * v)
            k += 1

    # Generate all primitive pairs: (m^2-n^2, 2mn+n^2)
    # with m > n >= 1, gcd(m,n) = 1, (m-n) % 3 != 0
    m = 2
    while m * m < LIMIT:  # rough bound
        for n in range(1, m):
            if gcd(m, n) != 1:
                continue
            if (m - n) % 3 == 0:
                continue
            a = m * m - n * n
            b = 2 * m * n + n * n
            add_primitive(a, b)
        m += 1

    # Find triangles: p > q > r >= 1, all three pairs compatible, p+q+r <= LIMIT
    valid_sums = set()

    for p in sorted(smaller.keys()):
        sp_set = smaller[p]
        for q in sorted(sp_set, reverse=True):
            max_r = LIMIT - p - q
            if max_r <= 0:
                continue
            if q not in smaller:
                continue
            for r in smaller[q]:
                if r >= q:
                    continue
                if r > max_r:
                    continue
                if r in sp_set:
                    valid_sums.add(p + q + r)

    answer = sum(valid_sums)
    print(answer)

if __name__ == '__main__':
    solve()
