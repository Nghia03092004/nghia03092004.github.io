"""
Problem 925: Continued Fraction Convergents

Compute p_n and q_n for the continued fraction expansion of sqrt(2),
with recurrence p_n = 2*p_{n-1} + p_{n-2}, q_n = 2*q_{n-1} + q_{n-2},
starting from p_0=1, p_1=3, q_0=1, q_1=2. Return (p_n + q_n) mod 10^9+7.

Key ideas:
    - sqrt(2) = [1; 2, 2, 2, ...] has all partial quotients = 2 after the first.
    - Convergents p_n/q_n approach sqrt(2) exponentially fast.
    - The recurrence is a linear second-order relation.
    - Ratio p_n/q_n oscillates above and below sqrt(2).

Methods:
    1. Modular arithmetic recurrence for large n
    2. Exact integer convergents for small n
    3. Convergence rate analysis
    4. Ratio of consecutive convergents (approaches 1 + sqrt(2))
"""

import numpy as np

def solve(n=100):
    """Compute (p_n + q_n) mod 10^9+7 via linear recurrence."""
    MOD = 10**9 + 7
    p0, p1 = 1, 3
    q0, q1 = 1, 2
    for i in range(2, n + 1):
        p0, p1 = p1, (2 * p1 + p0) % MOD
        q0, q1 = q1, (2 * q1 + q0) % MOD
    return (p1 + q1) % MOD

def convergents(n):
    """Return lists of exact (p_k, q_k) for k = 0..n."""
    ps = [1, 3]
    qs = [1, 2]
    for i in range(2, n + 1):
        ps.append(2 * ps[-1] + ps[-2])
        qs.append(2 * qs[-1] + qs[-2])
    return ps, qs

def convergence_errors(n):
    """Compute |p_k/q_k - sqrt(2)| for k = 0..n."""
    ps, qs = convergents(n)
    sqrt2 = np.sqrt(2)
    return [abs(ps[i] / qs[i] - sqrt2) for i in range(n + 1)]

def denominator_growth_ratio(n):
    """Compute q_{k+1}/q_k for k = 0..n-1, should approach 1+sqrt(2)."""
    _, qs = convergents(n)
    return [qs[i + 1] / qs[i] for i in range(n)]

# Solve and verify
answer = solve()

# Verify convergents approach sqrt(2) - errors decrease monotonically
ps, qs = convergents(20)
sqrt2 = np.sqrt(2)
errors_v = [abs(ps[i] / qs[i] - sqrt2) for i in range(1, 15)]
for i in range(len(errors_v) - 1):
    assert errors_v[i + 1] < errors_v[i], "Errors should decrease monotonically"

# Verify alternating property: even convergents < sqrt(2), odd > sqrt(2)
for i in range(2, 10):
    # Use integer comparison: p_i^2 vs 2*q_i^2
    if i % 2 == 0:
        assert ps[i]**2 < 2 * qs[i]**2, f"Even convergent {i} should be below sqrt(2)"
    else:
        assert ps[i]**2 > 2 * qs[i]**2, f"Odd convergent {i} should be above sqrt(2)"

# Verify determinant property: p_n*q_{n-1} - p_{n-1}*q_n = (-1)^(n+1)
for i in range(1, 20):
    det = ps[i] * qs[i - 1] - ps[i - 1] * qs[i]
    assert det == (-1)**(i + 1), f"Determinant property failed at {i}"

print(answer)
