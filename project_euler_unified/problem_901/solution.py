"""
Problem 901: Digital Root Patterns

Let d(n) denote the digital root of n (the single digit obtained by
repeatedly summing the digits of n). Define S(N) = sum_{k=1}^{N} k * d(k).
Find S(10^6) mod 10^9 + 7.

Key results:
    - Digital root: d(n) = 1 + (n-1) mod 9  for n >= 1
    - d(n) has period 9: pattern is 1,2,3,4,5,6,7,8,9,1,2,...
    - Each full block of 9 consecutive integers contributes a predictable sum
    - Closed-form via grouping into complete blocks of 9 + remainder

Methods:
    1. Closed-form via block decomposition (O(1) per block)
    2. Brute-force summation (verification)
    3. Incremental S(n) computation for visualization
"""

import numpy as np

def digital_root(n):
    """Digital root of n using the modular formula."""
    if n == 0:
        return 0
    return 1 + (n - 1) % 9

def solve_closed(N=10**6):
    """
    Closed-form computation of S(N) = sum(k * d(k) for k=1..N) mod 10^9+7.

    Group k=1..N into complete blocks of 9 consecutive integers.
    Block i (0-indexed) covers k = 9i+1 .. 9i+9.
    Within block i, for j=1..9: k = 9i+j, d(k) = j.
    Contribution = sum_{j=1}^{9} (9i+j)*j = 9i*45 + sum(j^2,j=1..9)
                 = 405i + 285.
    Sum over q complete blocks: 405*q*(q-1)/2 + 285*q.
    Then handle the partial remainder block.
    """
    MOD = 10**9 + 7
    q, r = divmod(N, 9)
    total = 405 * q * (q - 1) // 2 + 285 * q
    # Remainder: k = 9q+j for j=1..r
    for j in range(1, r + 1):
        total += (9 * q + j) * j
    return total % MOD

def solve_brute(N):
    """Brute force S(N) = sum(k * d(k) for k=1..N). For verification only."""
    return sum(k * digital_root(k) for k in range(1, N + 1))

def cumulative_s(N):
    """Return array where result[i] = S(i) for i=0..N."""
    s = np.zeros(N + 1, dtype=np.int64)
    for k in range(1, N + 1):
        s[k] = s[k - 1] + k * digital_root(k)
    return s

# Verification
for test_n in [27, 100, 1000]:
    assert solve_closed(test_n) == solve_brute(test_n), f"Mismatch at N={test_n}"

# Known: S(9) = 1*1+2*2+3*3+4*4+5*5+6*6+7*7+8*8+9*9 = 285
assert solve_closed(9) == 285
assert solve_closed(18) == 285 + (10*1+11*2+12*3+13*4+14*5+15*6+16*7+17*8+18*9)

# Solve
answer = solve_closed()
print(answer)
