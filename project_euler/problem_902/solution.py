"""
Problem 902: Permutation Cycles

Let C(n) be the number of permutations of {1,2,...,n} whose longest cycle
has length exactly floor(n/2). Find C(20) mod 10^9+7.

Key identity: C(n) = a(n, floor(n/2)) - a(n, floor(n/2) - 1)
where a(n, m) = number of permutations of [n] with all cycle lengths <= m.

The EGF for such permutations is:
    F_m(x) = exp(sum_{k=1}^{m} x^k / k)

Methods implemented:
    1. EGF polynomial truncation (exact rational arithmetic)
    2. Cycle-building DP recurrence
    3. Direct enumeration for small n (verification)
"""

from fractions import Fraction
from math import factorial, comb
from collections import Counter
import random

MOD = 10**9 + 7

def count_perms_max_cycle_le_egf(n: int, m: int):
    """Count permutations of [n] with all cycles <= m using EGF.

    F_m(x) = exp(sum_{k=1}^{m} x^k / k), then a(n,m) = n! * [x^n] F_m(x).
    We compute F_m(x) by multiplying exp(x^k/k) for k = 1..m.
    """
    poly = [Fraction(0)] * (n + 1)
    poly[0] = Fraction(1)
    for k in range(1, m + 1):
        new_poly = [Fraction(0)] * (n + 1)
        for i in range(n + 1):
            if poly[i] == 0:
                continue
            # Multiply by exp(x^k / k) = sum_{j>=0} x^{jk} / (k^j * j!)
            coeff = Fraction(1)
            for j in range(0, (n - i) // k + 1):
                new_poly[i + j * k] += poly[i] * coeff
                coeff /= (k * (j + 1))
        poly = new_poly
    return int(poly[n] * factorial(n))

def count_perms_max_cycle_le_dp(n: int, m: int):
    """Count permutations of [n] with all cycles <= m using DP.

    D(s) = sum_{k=1}^{min(s,m)} C(s-1, k-1) * (k-1)! * D(s-k)
         = sum_{k=1}^{min(s,m)} (s-1)! / (s-k)! * D(s-k)

    Element 1 is placed in a k-cycle: choose k-1 companions, arrange cyclically.
    """
    dp = [0] * (n + 1)
    dp[0] = 1
    for s in range(1, n + 1):
        total = 0
        falling = 1  # (s-1)! / (s-k)! for current k
        for k in range(1, min(s, m) + 1):
            if k > 1:
                falling *= (s - k + 1)
                falling //= (k - 1)
                # Actually: C(s-1,k-1)*(k-1)! = (s-1)!/(s-k)!
            # Recompute properly
            pass
        # Cleaner: direct falling factorial
        coeff = 1  # will be (s-1)!/(s-k)! = (s-1)(s-2)...(s-k+1)
        for k in range(1, min(s, m) + 1):
            if k == 1:
                coeff = 1
            else:
                coeff *= (s - k + 1)
            total += coeff * dp[s - k]
        dp[s] = total
    return dp[n]

def max_cycle_length(perm: list):
    """Find the longest cycle in a permutation."""
    n = len(perm)
    visited = [False] * n
    max_len = 0
    for i in range(n):
        if not visited[i]:
            length = 0
            j = i
            while not visited[j]:
                visited[j] = True
                j = perm[j]
                length += 1
            max_len = max(max_len, length)
    return max_len

def count_exact_brute(n: int, target: int):
    """Brute-force count of permutations with max cycle = target. Only for small n."""
    from itertools import permutations
    count = 0
    for perm in permutations(range(n)):
        if max_cycle_length(list(perm)) == target:
            count += 1
    return count

# Solve
N = 20
HALF = N // 2  # = 10

a_egf = count_perms_max_cycle_le_egf(N, HALF)
b_egf = count_perms_max_cycle_le_egf(N, HALF - 1)
ans_egf = (a_egf - b_egf) % MOD

a_dp = count_perms_max_cycle_le_dp(N, HALF)
b_dp = count_perms_max_cycle_le_dp(N, HALF - 1)
ans_dp = (a_dp - b_dp) % MOD

assert ans_egf == ans_dp, f"Method mismatch: EGF={ans_egf}, DP={ans_dp}"

# Cross-check: exact values should match
assert a_egf == a_dp, f"a mismatch: {a_egf} vs {a_dp}"
assert b_egf == b_dp, f"b mismatch: {b_egf} vs {b_dp}"

for test_n in [4, 6, 8]:
    target = test_n // 2
    a_bf = count_perms_max_cycle_le_egf(test_n, target)
    b_bf = count_perms_max_cycle_le_egf(test_n, target - 1)
    c_bf = a_bf - b_bf
    c_exact = count_exact_brute(test_n, target)
    assert c_bf == c_exact, f"n={test_n}: EGF={c_bf}, brute={c_exact}"

print(ans_egf)
