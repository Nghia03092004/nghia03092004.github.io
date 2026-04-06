"""
Problem 900: Sum of Remainders
Compute S(n) = sum_{k=1}^{n} (n mod k) using the Dirichlet hyperbola method.

Key identity: S(n) = n^2 - sum_{k=1}^{n} k * floor(n/k)
The floor function takes O(sqrt(n)) distinct values, enabling block summation.
"""

import math

def S_brute(n):
    """Brute force: O(n) computation of sum of remainders."""
    return sum(n % k for k in range(1, n + 1))

def S_hyperbola(n):
    """
    O(sqrt(n)) computation via floor-block decomposition.
    Groups k by the value v = floor(n/k) and sums k*v over each block.
    """
    T = 0
    k = 1
    while k <= n:
        v = n // k
        kp = n // v  # last k in this block
        # sum of k, k+1, ..., kp = (kp - k + 1)*(k + kp) // 2
        block_sum = (kp - k + 1) * (k + kp) // 2
        T += v * block_sum
        k = kp + 1
    return n * n - T

def S_divisor_sum(n):
    """
    Alternative via sigma function: T(n) = sum_{m=1}^{n} sigma(m).
    O(n sqrt(n)) -- for verification only.
    """
    T = 0
    for m in range(1, n + 1):
        for d in range(1, int(math.isqrt(m)) + 1):
            if m % d == 0:
                T += d
                if d != m // d:
                    T += m // d
    return n * n - T

# --- Verification ---
print("=== Verification: S(n) by three methods ===")
print(f"{'n':>6} {'Brute':>10} {'Hyperbola':>10} {'DivSum':>10} {'Match':>6}")
for n in [1, 2, 5, 10, 20, 50, 100, 200, 500, 1000]:
    sb = S_brute(n)
    sh = S_hyperbola(n)
    sd = S_divisor_sum(n)
    match = "OK" if sb == sh == sd else "FAIL"
    print(f"{n:>6} {sb:>10} {sh:>10} {sd:>10} {match:>6}")
    assert sb == sh == sd, f"Mismatch at n={n}: {sb}, {sh}, {sd}"

# Large computation
for n in [10**6, 10**7, 10**8]:
    val = S_hyperbola(n)
    ratio = val / (n * n)
    print(f"S({n:.0e}) = {val}, ratio S/n^2 = {ratio:.6f}")

print(f"\nTheoretical limit: 1 - pi^2/12 = {1 - math.pi**2/12:.6f}")

answer = S_hyperbola(10**8)
print(f"\nAnswer: S(10^8) = {answer}")

# --- 4-Panel Visualization ---
