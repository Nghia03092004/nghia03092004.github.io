"""
Project Euler Problem 396: Weak Goodstein Sequence

Find the last 9 digits of sum of G(n) for 1 <= n < 16,
where G(n) is the number of nonzero elements in the nth weak Goodstein sequence.

Key insight: G(n) can be computed via hierarchical function decomposition.
The weak Goodstein sequence digit subtraction maps to iterated exponential
functions computable mod M using Euler's totient for tower-of-powers arithmetic.
"""

import sys
sys.setrecursionlimit(100000)

MOD = 10**9

def phi_func(x):
    """Euler's totient for numbers of form 10^9 / 2^a / 5^b."""
    if x % 2 == 0:
        x //= 2
    if x % 5 == 0:
        x = x // 5 * 4
    return x

memo_f3 = {}

def f3(d, t, M):
    """Compute deeply recursive tower-of-powers function mod M."""
    if M == 1:
        return 0
    key = (d, t, M)
    if key in memo_f3:
        return memo_f3[key]
    if d == 0:
        return t % M
    p = phi_func(M)
    x = f3(d - 1, t, p)
    result = pow(2, x, M) * f3(d - 1, t, M) % M
    memo_f3[key] = result
    return result

def f(x, t, M):
    """Compute x-th level function mod M."""
    if M == 1:
        return 0
    if x == 0:
        return (t + 1) % M
    if x == 1:
        return t * 2 % M
    if x == 2:
        return pow(2, t, M) * t % M
    if x == 3:
        return f3(t, t, M)
    raise ValueError(f"x={x} not supported")

def g(x):
    """Compute G(x) mod MOD using binary decomposition of x."""
    t = 3
    for i in range(5):
        if x >> i & 1:
            t = f(i, t, MOD)
    return t - 3

def solve():
    ans = 0
    for i in range(1, 16):
        gi = g(i)
        ans = (ans + gi) % MOD
    print(ans)

if __name__ == "__main__":
    solve()
