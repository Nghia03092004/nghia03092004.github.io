"""
Problem 970: Collatz Stopping Time Statistics

Compute the sum of total stopping times t(n) for n = 1 to 10^6.
t(n) is the number of steps in the Collatz sequence until reaching 1.

Key results:
    - Collatz rule: if even, n -> n/2; if odd, n -> 3n+1
    - Use memoization: once we reach a value < n, reuse cached result
    - answer = sum of t(n) for n = 1..10^6

Methods:
    1. solve_collatz_memoized — DP with memoization for stopping times
    2. collatz_sequence       — full trajectory for a single starting value
    3. max_stopping_time      — find n with largest t(n) up to N
    4. average_by_decade      — mean stopping time per order of magnitude
"""

import numpy as np

def solve_collatz_memoized(N):
    """Return (total_sum, t_array) where t[n] = stopping time for n."""
    t = [0] * (N + 1)
    for n in range(2, N + 1):
        k = n
        steps = 0
        while k >= n:
            if k % 2 == 0:
                k //= 2
            else:
                k = 3 * k + 1
            steps += 1
        t[n] = steps + t[k]
    return sum(t), t

def collatz_sequence(n):
    """Return the full Collatz trajectory from n down to 1."""
    seq = [n]
    while n != 1:
        if n % 2 == 0:
            n //= 2
        else:
            n = 3 * n + 1
        seq.append(n)
    return seq

def max_stopping_time(t, N):
    """Return (n_max, t_max) for n in 1..N."""
    t_max = 0
    n_max = 1
    for n in range(1, N + 1):
        if t[n] > t_max:
            t_max = t[n]
            n_max = n
    return n_max, t_max

def average_by_decade(t, max_exp=6):
    """Mean stopping time for n in [10^(k-1), 10^k) for k=1..max_exp."""
    decades = []
    means = []
    for k in range(1, max_exp + 1):
        lo = 10 ** (k - 1)
        hi = min(10 ** k, len(t))
        vals = t[lo:hi]
        decades.append(k)
        means.append(np.mean(vals) if vals else 0)
    return decades, means

#  Verification
# Known: t(1)=0, t(2)=1, t(3)=7, t(4)=2, t(6)=8, t(7)=16
_s, _t = solve_collatz_memoized(20)
assert _t[1] == 0
assert _t[2] == 1
assert _t[3] == 7
assert _t[4] == 2
assert _t[6] == 8
assert _t[7] == 16

#  Main computation
N = 10 ** 6
answer, t = solve_collatz_memoized(N)
print(answer)

n_max, t_max = max_stopping_time(t, N)

#  Visualization — 4-panel figure
