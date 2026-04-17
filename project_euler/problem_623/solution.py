"""
Problem 623: Lambda Count

Count permutations of {1,...,n} with exactly 2 disjoint cycles.
This equals the unsigned Stirling number [n,2] = (n-1)! * H_{n-1}.

Key identity: [n,2] = (n-1)! * sum_{k=1}^{n-1} 1/k

Method 1: Direct formula (n-1)! * H_{n-1} with modular inverses
Method 2: Recurrence [n,2] = (n-1)*[n-1,2] + (n-2)!
Method 3: Exact computation for verification
"""

from math import factorial
from fractions import Fraction

MOD = 10**9 + 7

# --- Method 1: Direct formula with modular arithmetic ---
def lambda_mod_direct(n: int, mod: int):
    """Compute [n,2] = (n-1)! * H_{n-1} mod p using Fermat inverses."""
    if n < 2:
        return 0
    fact = 1
    harmonic = 0
    for k in range(1, n):
        fact = fact * k % mod
        harmonic = (harmonic + pow(k, mod - 2, mod)) % mod
    return fact * harmonic % mod

# --- Method 1b: Using fast inverse recurrence ---
def lambda_mod_fast(n: int, mod: int):
    """Compute [n,2] = (n-1)! * H_{n-1} mod p using inverse recurrence.

    inv[k] = -(p mod k)^{-1} * floor(p/k) mod p
    """
    if n < 2:
        return 0
    inv = [0] * n
    inv[1] = 1
    for k in range(2, n):
        inv[k] = -(mod // k) * inv[mod % k] % mod

    fact = 1
    harmonic = 0
    for k in range(1, n):
        fact = fact * k % mod
        harmonic = (harmonic + inv[k]) % mod
    return fact * harmonic % mod

# --- Method 2: Recurrence ---
def lambda_mod_recurrence(n: int, mod: int):
    """Compute [n,2] via recurrence: [n,2] = (n-1)*[n-1,2] + (n-2)!"""
    if n < 2:
        return 0
    stir = 0  # [2,2] = 1 is handled at n=2
    fact_nm2 = 1  # (n-2)! starting at n=2: 0! = 1
    stir = 1  # [2,2]

    for m in range(3, n + 1):
        fact_nm2 = fact_nm2 * (m - 2) % mod  # (m-2)!
        stir = ((m - 1) * stir + fact_nm2) % mod
    return stir if n >= 2 else 0

# --- Method 3: Exact computation ---
def lambda_exact(n: int):
    """Compute exact [n,2] using Fraction arithmetic."""
    if n < 2:
        return 0
    h = Fraction(0)
    for k in range(1, n):
        h += Fraction(1, k)
    return int(factorial(n - 1) * h)

# Compute and verify

# Exact values for small n
expected = {2: 1, 3: 3, 4: 11, 5: 50, 6: 274, 7: 1764, 8: 13068, 9: 109584, 10: 1026576}

for n, val in expected.items():
    exact = lambda_exact(n)
    assert exact == val, f"Exact: [n={n},2] = {exact}, expected {val}"
    mod_dir = lambda_mod_direct(n, MOD)
    mod_fast = lambda_mod_fast(n, MOD)
    mod_rec = lambda_mod_recurrence(n, MOD)
    assert mod_dir == val % MOD
    assert mod_fast == val % MOD
    assert mod_rec == val % MOD

# Verify three methods agree for larger n
for n in range(2, 500):
    v1 = lambda_mod_direct(n, MOD)
    v2 = lambda_mod_fast(n, MOD)
    v3 = lambda_mod_recurrence(n, MOD)
    assert v1 == v2 == v3, f"Mismatch at n={n}: {v1}, {v2}, {v3}"

# Verify combinatorial interpretation: [3,2] = 3
# The 3 permutations with 2 cycles: (1)(23), (2)(13), (3)(12)
from itertools import permutations

def count_cycles(perm):
    n = len(perm)
    visited = [False] * n
    cycles = 0
    for i in range(n):
        if not visited[i]:
            cycles += 1
            j = i
            while not visited[j]:
                visited[j] = True
                j = perm[j]
    return cycles

for n in range(2, 8):
    count = sum(1 for p in permutations(range(n)) if count_cycles(p) == 2)
    assert count == lambda_exact(n), f"n={n}: counted {count}, formula gives {lambda_exact(n)}"

print("All verifications passed.")

# Print table
print(f"\n{'n':>4} {'[n,2]':>12} {'(n-1)!':>10} {'H_{n-1}':>12}")
for n in range(2, 11):
    val = lambda_exact(n)
    fact = factorial(n - 1)
    h = Fraction(0)
    for k in range(1, n):
        h += Fraction(1, k)
    print(f"{n:>4} {val:>12} {fact:>10} {str(h):>12}")

# Specific answer for n = 10^6 mod 10^9 + 7
n_large = 10**6
ans = lambda_mod_fast(n_large, MOD)
print(f"\n[{n_large}, 2] mod {MOD} = {ans}")
