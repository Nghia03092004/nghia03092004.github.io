"""
Problem 730: Shifted Pythagorean Triples

p^2 + q^2 + k = r^2, gcd(p,q,r)=1, 1<=p<=q<=r, p+q+r<=n.
P_k(n) = count of such primitive triples.
S(m, n) = sum_{k=0}^{m} P_k(n).
"""

from math import gcd, isqrt

def divisors(n):
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

def P_k(k, n):
    """Count primitive k-shifted Pythagorean triples with p+q+r <= n."""
    count = 0
    for p in range(1, n // 3 + 1):
        val = p * p + k
        for d1 in divisors(val):
            d2 = val // d1
            if d1 > d2:
                break
            if (d1 % 2) != (d2 % 2):
                continue
            q = (d2 - d1) // 2
            r = (d1 + d2) // 2
            if q < p:
                continue
            if p + q + r > n:
                continue
            if gcd(gcd(p, q), r) != 1:
                continue
            count += 1
    return count

def S(m, n):
    """Compute S(m, n) = sum_{k=0}^{m} P_k(n)."""
    return sum(P_k(k, n) for k in range(m + 1))

# Verify
print(f"P_0(10^4) = {P_k(0, 10000)}")  # Expected: 703
print(f"P_20(10^4) = {P_k(20, 10000)}")  # Expected: 1979
print(f"S(10, 10^4) = {S(10, 10000)}")  # Expected: 10956

# Full answer S(100, 10^8) requires optimized implementation
