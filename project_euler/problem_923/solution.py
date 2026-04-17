"""
Problem 923: Combinatorial Game Values

Nim heaps 1..n, G(n) = XOR(1..n). Count n <= 10^6 with G(n) = 0.

Key ideas:
    - Sprague-Grundy: position loses iff XOR of heaps = 0.
    - XOR(1..n) period-4: n, 1, n+1, 0 for n%4 = 0,1,2,3.
    - G(n) = 0 iff n = 3 mod 4.
    - Count = (N-3)//4 + 1 = 250000.

Methods:
    1. Closed-form counting via modular arithmetic
    2. Brute-force XOR prefix verification
    3. Cumulative win-rate analysis
    4. Period pattern decomposition
"""

from collections import Counter

N = 10**6

def xor_prefix(n):
    """Compute XOR(1..n) using the period-4 closed form."""
    r = n % 4
    if r == 0: return n
    if r == 1: return 1
    if r == 2: return n + 1
    return 0

def count_zero_xor(N):
    """Count how many n in [1..N] have XOR(1..n) = 0."""
    return (N - 3) // 4 + 1 if N >= 3 else 0

def count_brute(N):
    """Count G(n) = 0 by iterating and computing running XOR."""
    count = 0
    xor_val = 0
    for n in range(1, N + 1):
        xor_val ^= n
        if xor_val == 0:
            count += 1
    return count

def zero_positions(N):
    """Return list of n where G(n) = 0."""
    return [n for n in range(1, N + 1) if n % 4 == 3]

# Solve and verify
answer = count_zero_xor(N)

# Verify closed-form against brute force
for test_N in [10, 100, 1000, 10000]:
    assert count_zero_xor(test_N) == count_brute(test_N), f"Mismatch at N={test_N}"

# Verify XOR prefix formula
for n in range(1, 200):
    expected = 0
    for k in range(1, n + 1):
        expected ^= k
    assert xor_prefix(n) == expected, f"XOR prefix mismatch at n={n}"

# Verify zero positions are exactly n = 3 mod 4
zp = zero_positions(200)
assert all(n % 4 == 3 for n in zp)
assert len(zp) == count_zero_xor(200)

print(answer)
