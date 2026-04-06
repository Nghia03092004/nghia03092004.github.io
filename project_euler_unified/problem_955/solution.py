"""
Problem 955: Fibonacci Entry Points

For each m = 1, ..., 10^5, find alpha(m) = smallest k >= 1 with m | F_k.
Return sum of all alpha(m).

Key properties:
  - alpha(m) divides the Pisano period pi(m)
  - For coprime a,b: alpha(ab) = lcm(alpha(a), alpha(b))
  - alpha(p^a) = p^{a-1} * alpha(p) for primes p

Complexity: O(N * avg_alpha) ~ O(N^2) direct, O(N^{3/2}) with factorization.
"""

from math import gcd

def fibonacci_entry_point(m: int) -> int:
    """Find smallest k >= 1 such that F_k is divisible by m."""
    if m == 1:
        return 1
    a, b = 0, 1
    for k in range(1, 6 * m + 2):
        a, b = b, (a + b) % m
        if a == 0:
            return k
    raise ValueError(f"Entry point not found for m={m}")

def solve(N: int = 10**5) -> int:
    """Compute sum of alpha(m) for m = 1 to N."""
    return sum(fibonacci_entry_point(m) for m in range(1, N + 1))

# --- Compute answer ---
N = 10**5
total = 0
alphas = [0] * (N + 1)
for m in range(1, N + 1):
    alphas[m] = fibonacci_entry_point(m)
    total += alphas[m]

# Verify known values
assert alphas[1] == 1
assert alphas[2] == 3
assert alphas[3] == 4
assert alphas[5] == 5
assert alphas[7] == 8

print(total)
