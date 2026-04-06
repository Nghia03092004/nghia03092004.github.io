"""
Project Euler Problem 527: Randomized Binary Search

R(n) = 2*H_n - 1 where H_n is the n-th harmonic number.
B(n) = average depth in standard binary search tree on n elements.
Find R(10^10) - B(10^10) rounded to 8 decimal places.
"""

import math

def harmonic_asymptotic(n):
    """Compute H_n using Euler-Maclaurin asymptotic expansion."""
    x = float(n)
    gamma = 0.5772156649015328606
    result = math.log(x) + gamma
    inv_n = 1.0 / x
    inv_n2 = inv_n * inv_n

    # Bernoulli number corrections
    result += 0.5 * inv_n
    result -= inv_n2 / 12.0
    result += inv_n2 * inv_n2 / 120.0
    result -= inv_n2 * inv_n2 * inv_n2 / 252.0
    result += inv_n2 * inv_n2 * inv_n2 * inv_n2 / 240.0
    result -= inv_n2**5 / 132.0
    result += 691.0 * inv_n2**6 / 32760.0

    return result

def compute_B_total_depth(n):
    """
    Compute total depth of all nodes in the implicit binary search tree of size n.
    The root (at position ceil(n/2)) has depth 1.
    Uses recursion with memoization.
    """
    memo = {}

    def total_depth(n):
        if n <= 0:
            return 0.0
        if n == 1:
            return 1.0
        if n in memo:
            return memo[n]

        mid = (n + 1) // 2  # root position
        left_size = mid - 1
        right_size = n - mid

        left_sum = total_depth(left_size)
        right_sum = total_depth(right_size)

        # Root depth=1, children depths are parent+1
        result = 1.0 + (left_sum + left_size) + (right_sum + right_size)
        memo[n] = result
        return result

    return total_depth(n)

def R_brute(n):
    """Brute force R(n) for verification."""
    R = [0.0] * (n + 1)
    R[1] = 1.0
    for k in range(2, n + 1):
        s = sum(j * R[j] for j in range(1, k))
        R[k] = 1.0 + 2.0 * s / (k * k)
    return R[n]

# Verify small cases
print(f"R(6) = {R_brute(6):.8f}")   # 2.71666667

td6 = compute_B_total_depth(6)
print(f"B(6) = {td6/6:.8f}")         # 2.33333333

# Compute for 10^10
N = 10**10

# R(N) = 2*H_N - 1
H_N = harmonic_asymptotic(N)
R_N = 2.0 * H_N - 1.0

# B(N) via recursive tree decomposition
total_depth_N = compute_B_total_depth(N)
B_N = total_depth_N / N

answer = R_N - B_N
print(f"R(10^10) - B(10^10) = {answer:.8f}")
