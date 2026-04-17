"""
Problem 531: Chinese Leftovers

Compute S = sum of g(phi(n), n, phi(m), m) over 1000000 <= n < m <= 1005000,
where g(a,n,b,m) is the smallest non-negative solution to the system
x = a (mod n), x = b (mod m), or 0 if none exists.

Method: Extended CRT via Bezout's identity; totient sieve for phi values.
"""

from math import gcd

def extended_gcd(a, b):
    """Return (g, x, y) such that a*x + b*y = g = gcd(a, b)."""
    if a == 0:
        return b, 0, 1
    g, x, y = extended_gcd(b % a, a)
    return g, y - (b // a) * x, x

def crt(a, n, b, m):
    """Smallest non-negative x with x = a (mod n), x = b (mod m), or 0."""
    d = gcd(n, m)
    if (a - b) % d != 0:
        return 0
    lcm = n // d * m
    _, u, _ = extended_gcd(n, m)
    x = (a + n * ((b - a) // d) * u) % lcm
    return x

def euler_totient_sieve(limit):
    """Compute phi(k) for k = 0..limit via the Euler product sieve."""
    phi = list(range(limit + 1))
    for i in range(2, limit + 1):
        if phi[i] == i:  # i is prime
            for j in range(i, limit + 1, i):
                phi[j] -= phi[j] // i
    return phi

LO, HI = 1000000, 1005000
phi = euler_totient_sieve(HI)

S = 0
for n in range(LO, HI + 1):
    for m in range(n + 1, HI + 1):
        S += crt(phi[n], n, phi[m], m)

print(S)
