"""
Problem 738: Counting Ordered Factorisations

d(n,k) = ways to write n as product of k non-decreasing integers >= 1. D(N,K) = sum.
"""

MOD = 10**9 + 7

def d_brute(n, k):
    """Count non-decreasing factorizations n = x1*x2*...*xk, xi >= 1."""
    if k == 1:
        return 1
    count = 0
    # x1 ranges from 1 to n, x1 <= x2 <= ... <= xk
    def helper(remaining, num_left, min_val):
        if num_left == 1:
            return 1 if remaining >= min_val else 0
        total = 0
        d = min_val
        while d * d <= remaining or (num_left == 1 and d <= remaining):
            if remaining % d == 0:
                total += helper(remaining // d, num_left - 1, d)
            d += 1
            if d > remaining:
                break
        # Also check if remaining itself works as the next factor
        if num_left == 1 and remaining >= min_val:
            pass  # handled above
        return total
    return helper(n, k, 1)

def D_brute(N, K):
    """Compute D(N, K) by brute force."""
    total = 0
    for n in range(1, N + 1):
        for k in range(1, K + 1):
            total += d_brute(n, k)
    return total

# Verify
d10_10 = D_brute(10, 10)
print(f"D(10, 10) = {d10_10}")  # Expected: 153

d100_100 = D_brute(100, 100)
print(f"D(100, 100) = {d100_100}")  # Expected: 35384
