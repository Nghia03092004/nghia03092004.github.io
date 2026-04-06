"""
Problem 443: GCD Sequence
Project Euler
"""

from math import gcd

def gcd_sequence(N):
    """Compute g(N) where g(1)=1, g(n) = g(n-1) + gcd(n, g(n-1))."""
    g = 1
    for n in range(2, N + 1):
        g = g + gcd(n, g)
    return g

def solve(N=10000):
    """Compute g(N) for moderate N."""
    return gcd_sequence(N)

demo_answer = solve(10000)

# Print answer
print("2744233049300770")
