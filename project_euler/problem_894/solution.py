"""
Problem 894: Summation of Summations
m-fold iterated partial sum: S^(m)_n = sum_k C(n-k+m-1, m-1) * a_k
Special case a_k = k: S^(m)_n = C(n+m, m+1).
"""

from math import comb

def iterated_sum_brute(a, m):
    """Compute m-fold iterated partial sum by nested iteration."""
    n = len(a)
    current = list(a)
    for _ in range(m):
        prefix = [0] * n
        prefix[0] = current[0]
        for i in range(1, n):
            prefix[i] = prefix[i - 1] + current[i]
        current = prefix
    return current[-1]

def iterated_sum_formula(a, m):
    """Compute using closed-form: sum_k C(n-k+m-1, m-1) * a_k."""
    n = len(a)
    total = 0
    for k in range(n):
        weight = comb(n - 1 - k + m - 1, m - 1)
        total += weight * a[k]
    return total

def iterated_sum_prefix(a, m):
    """Compute by applying prefix sum m times. O(mn)."""
    current = list(a)
    for _ in range(m):
        for i in range(1, len(current)):
            current[i] += current[i - 1]
    return current[-1]

def iterated_sum_identity(n, m):
    """For a_k = k: S^(m)_n = C(n+m, m+1)."""
    return comb(n + m, m + 1)

# --- Verification ---
print("=== Triple Summation (m=3) Verification ===")
for n in range(1, 11):
    a = list(range(1, n + 1))  # a_k = k
    brute = iterated_sum_brute(a, 3)
    formula = iterated_sum_formula(a, 3)
    prefix = iterated_sum_prefix(a, 3)
    identity = iterated_sum_identity(n, 3)
    match = "OK" if brute == formula == prefix == identity else "FAIL"
    print(f"  n={n:>2}: brute={brute:>6}, formula={formula:>6}, "
          f"prefix={prefix:>6}, C(n+3,4)={identity:>6}  {match}")
    assert brute == formula == prefix == identity

print("\n=== a_k = 1 (m=2) ===")
for n in range(1, 11):
    a = [1] * n
    result = iterated_sum_formula(a, 2)
    expected = comb(n + 1, 2)
    print(f"  n={n}: S^(2) = {result}, C(n+1,2) = {expected}, "
          f"Match: {'OK' if result == expected else 'FAIL'}")
    assert result == expected

print("\n=== General sequence verification ===")
a = [3, 1, 4, 1, 5, 9, 2, 6]
for m in range(1, 6):
    brute = iterated_sum_brute(a, m)
    formula = iterated_sum_formula(a, m)
    prefix = iterated_sum_prefix(a, m)
    print(f"  m={m}: brute={brute}, formula={formula}, prefix={prefix}, "
          f"Match: {'OK' if brute == formula == prefix else 'FAIL'}")
    assert brute == formula == prefix

print("\n=== Weight Table (m=3, n=5) ===")
n = 5
print(f"{'k':>3} {'C(n-k+2,2)':>12} {'Contribution (a_k=k)':>22}")
for k in range(1, n + 1):
    w = comb(n - k + 2, 2)
    print(f"{k:>3} {w:>12} {w * k:>22}")

# Large computation
n_large = 10000
m_large = 3
a_large = list(range(1, n_large + 1))
result = iterated_sum_identity(n_large, m_large)
print(f"\nS^(3)_{{10000}} for a_k=k: C(10003, 4) = {result}")

answer = result
print(f"\nAnswer: {answer}")

# --- 4-Panel Visualization ---
