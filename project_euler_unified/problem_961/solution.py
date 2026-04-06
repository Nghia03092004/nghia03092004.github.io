"""
Problem 961: Repunit Divisibility

R_n = (10^n - 1) / 9 = 111...1 (n ones).
Find sum of all n <= 1000 such that n | R_n.

Equivalently: 9n | (10^n - 1), i.e., 10^n = 1 (mod 9n).
Only possible when gcd(n, 10) = 1 (n odd, not divisible by 5).

Complexity: O(N log N) with modular exponentiation.
"""

from math import gcd

def is_repunit_divisible(n: int) -> bool:
    """Check if n divides R_n = (10^n - 1) / 9."""
    if n == 1:
        return True
    if gcd(n, 10) > 1:
        return False
    return pow(10, n, 9 * n) == 1

def solve(N: int = 1000) -> tuple[int, list[int]]:
    """Find sum of all n <= N with n | R_n."""
    qualifying = [n for n in range(1, N + 1) if is_repunit_divisible(n)]
    return sum(qualifying), qualifying

# --- Compute answer ---
answer, qualifying = solve(1000)

# Verify small cases
assert is_repunit_divisible(1)  # R_1 = 1, 1|1
assert is_repunit_divisible(3)  # R_3 = 111 = 37*3
assert is_repunit_divisible(9)  # R_9 = 111111111 / 9 = 12345679... 9 | 111111111
assert not is_repunit_divisible(2)
assert not is_repunit_divisible(5)

print(f"Qualifying values: {qualifying}")
print(answer)
