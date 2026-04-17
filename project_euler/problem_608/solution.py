"""
Problem 608: Divisor Sums Modulo Prime
Compute sums involving divisor functions modulo a prime.
"""

def sigma(n, k=1):
    """Sum of k-th powers of divisors of n."""
    total = 0
    for d in range(1, int(n**0.5) + 1):
        if n % d == 0:
            total += d**k
            if d != n // d:
                total += (n // d)**k
    return total

def sigma0(n):
    """Number of divisors of n."""
    return sigma(n, 0)

def solve(N=1000, p=1000000007):
    """Sum of sigma(n) for n=1..N, mod p."""
    total = 0
    for n in range(1, N + 1):
        total = (total + sigma(n)) % p
    return total

# Verify
assert sigma(1) == 1
assert sigma(6) == 12  # 1+2+3+6
assert sigma(12) == 28  # 1+2+3+4+6+12
assert sigma0(12) == 6

answer = solve(10000)
print(f"Sum of sigma(n) for n=1..10000 mod 10^9+7: {answer}")
