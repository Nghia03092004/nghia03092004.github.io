"""
Problem 561: Divisor Pairs

Compute S(N) = sum_{n=1}^{N} Pairs(n), where Pairs(n) = ceil(tau(n)/2).

By Lemma 1: S(N) = (sum_tau + floor(sqrt(N))) / 2.
By the Dirichlet hyperbola method: sum_tau = 2 * T - M^2,
where T = sum_{d=1}^{M} floor(N/d) and M = floor(sqrt(N)).
T is evaluated in O(sqrt(N)) via block decomposition of floor(N/d).
"""

from math import isqrt

def solve(N):
    M = isqrt(N)
    # Ensure M = floor(sqrt(N)) exactly
    while (M + 1) * (M + 1) <= N:
        M += 1
    while M * M > N:
        M -= 1

    # Block decomposition: sum_{d=1}^{M} floor(N/d)
    T = 0
    d = 1
    while d <= M:
        q = N // d
        d_max = min(N // q, M)
        T += q * (d_max - d + 1)
        d = d_max + 1

    sum_tau = 2 * T - M * M
    return (sum_tau + M) // 2

print(solve(10**10))
