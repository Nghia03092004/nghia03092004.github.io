"""
Problem 984: Bell Number B_1000 mod 10^9+7

Compute the 1000th Bell number modulo 10^9 + 7 using the Bell triangle.

Bell numbers B_n count the number of partitions of a set of n elements.
    B_0 = 1, B_1 = 1, B_2 = 2, B_3 = 5, B_4 = 15, B_5 = 52, ...

The Bell triangle recurrence:
    a(0,0) = 1
    a(n,0) = a(n-1, n-1)        (left edge = previous row's last element)
    a(n,k) = a(n,k-1) + a(n-1,k-1)
    B_n = a(n,0)

Key properties:
    - B_n grows super-exponentially: B_n ~ (n / W(n))^n / e^n
    - The Bell triangle modular computation uses O(n) space with rolling rows
    - Dobinski's formula: B_n = (1/e) * sum_{k=0}^{inf} k^n / k!

Answer: computed via Bell triangle mod 10^9+7

Methods:
    - bell_triangle_mod(n, mod): Bell number via triangle with modular arithmetic
    - bell_exact_small(n): Exact Bell numbers for small n (verification)
    - stirling2(n, k): Stirling numbers of second kind (B_n = sum S(n,k))
    - dobinski_approx(n): Approximate B_n via Dobinski's formula
"""

from math import factorial, comb

MOD = 10**9 + 7


def bell_triangle_mod(n, mod):
    """Compute B_n mod m using the Bell triangle with rolling array."""
    if n == 0:
        return 1
    row = [0] * (n + 1)
    row[0] = 1
    for i in range(1, n + 1):
        new_row = [0] * (n + 1)
        new_row[0] = row[i - 1]
        for j in range(1, i + 1):
            new_row[j] = (new_row[j - 1] + row[j - 1]) % mod
        row = new_row
    return row[0]


def bell_exact_small(n_max):
    """Compute exact Bell numbers B_0..B_{n_max} for small n."""
    B = [0] * (n_max + 1)
    B[0] = 1
    # Using Stirling: B_n = sum_{k=0}^{n} S(n,k)
    S = [[0] * (n_max + 1) for _ in range(n_max + 1)]
    S[0][0] = 1
    for nn in range(1, n_max + 1):
        for kk in range(1, nn + 1):
            S[nn][kk] = kk * S[nn - 1][kk] + S[nn - 1][kk - 1]
        B[nn] = sum(S[nn])
    return B


def stirling2_row(n):
    """Compute S(n, k) for k=0..n."""
    if n == 0:
        return [1]
    S = [0] * (n + 1)
    S[0] = 0
    prev = stirling2_row(n - 1) + [0]
    for k in range(1, n + 1):
        S[k] = k * prev[k] + prev[k - 1]
    return S


def dobinski_approx(n, terms=200):
    """Approximate B_n using Dobinski's formula (for small n only)."""
    from math import exp
    total = 0.0
    for k in range(terms):
        total += k ** n / factorial(k) if k > 0 or n == 0 else 1.0 / factorial(0)
    return total / exp(1)

# Verification

# Known Bell numbers
known_bells = [1, 1, 2, 5, 15, 52, 203, 877, 4140, 21147, 115975]
exact_bells = bell_exact_small(10)

for i, b in enumerate(known_bells):
    assert exact_bells[i] == b, f"B_{i}: expected {b}, got {exact_bells[i]}"
    assert bell_triangle_mod(i, MOD) == b % MOD, f"Mod mismatch at B_{i}"

# Verify Dobinski for small values
for i in range(8):
    approx = dobinski_approx(i)
    assert abs(approx - known_bells[i]) < 0.5, f"Dobinski approx failed at B_{i}"

answer = bell_triangle_mod(1000, MOD)
print(answer)
