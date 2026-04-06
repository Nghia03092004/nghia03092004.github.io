"""
Project Euler Problem 430: Range Flips

N disks, initially white. Each turn, random A,B in [1,N], flip disks
from min(A,B) to max(A,B). Find E(10^10, 4000) rounded to 2 decimal places.

Key formula:
E(N,M) = N/2 + 1/2 * sum_{i=1}^{N} (1-2*p_i)^M

where p_i = 1 - ((i-1)^2 + (N-i)^2) / N^2

For large N, using asymptotic expansion:
E(N,M) ~ N*(M+1)/(2M+1) - M/(2M-1)
"""
from decimal import Decimal, getcontext

def E_exact_small(N, M):
    """Exact computation for small N (for verification)."""
    total = 0.0
    for i in range(1, N + 1):
        p_i = 1.0 - ((i - 1)**2 + (N - i)**2) / N**2
        r_i = 1.0 - 2.0 * p_i
        total += (1.0 + r_i**M) / 2.0
    return total

def E_large(N, M):
    """Asymptotic formula for large N."""
    getcontext().prec = 50
    N = Decimal(N)
    M = Decimal(M)
    # Leading term
    E_val = N * (M + 1) / (2 * M + 1)
    # First correction
    E_val -= M / (2 * M - 1)
    return float(E_val)

# Verify with test cases
print(f"E(3, 1) = {E_exact_small(3, 1):.6f} (expected 10/9 = {10/9:.6f})")
print(f"E(3, 2) = {E_exact_small(3, 2):.6f} (expected 5/3 = {5/3:.6f})")
print(f"E(10, 4) = {E_exact_small(10, 4):.6f} (expected ~5.157)")
print(f"E(100, 10) = {E_exact_small(100, 10):.6f} (expected ~51.893)")

# Final answer
ans = E_large(10**10, 4000)
print(f"\nE(10^10, 4000) = {ans:.2f}")
