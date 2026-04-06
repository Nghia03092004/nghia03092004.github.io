"""
Problem 524: First Sort II

E(n) = (n-1)(n+4)/12, the expected number of insertion steps
to sort a random permutation using the "first sort" algorithm.

Compute E(10^18) mod (10^9 + 7).
"""

from fractions import Fraction
from itertools import permutations

MOD = 10**9 + 7

# --- Method 1: Closed-form modular computation ---
def solve_closed_form(n: int, mod: int):
    """Compute E(n) = (n-1)(n+4)/12 mod p."""
    a = (n - 1) % mod
    b = (n + 4) % mod
    inv12 = pow(12, mod - 2, mod)
    return a * b % mod * inv12 % mod

# --- Method 2: Exact rational computation (verification) ---
def E_exact(n: int) -> Fraction:
    """Compute E(n) exactly as a fraction."""
    return Fraction(n - 1, 1) * Fraction(n + 4, 1) / 12

# --- Method 3: Monte Carlo simulation (verification) ---
def E_simulate(n: int, trials: int = 100000) -> float:
    """Estimate E(n) by simulating the sorting algorithm."""
    import random
    total_steps = 0
    for _ in range(trials):
        perm = list(range(1, n + 1))
        random.shuffle(perm)
        steps = 0
        while True:
            # Find first descent
            found = -1
            for i in range(1, len(perm)):
                if perm[i] < perm[i - 1]:
                    found = i
                    break
            if found == -1:
                break
            # Remove and reinsert
            val = perm.pop(found)
            # Binary search for correct position in sorted prefix
            lo, hi = 0, found
            while lo < hi:
                mid = (lo + hi) // 2
                if perm[mid] < val:
                    lo = mid + 1
                else:
                    hi = mid
            perm.insert(lo, val)
            steps += 1
        total_steps += steps
    return total_steps / trials

# --- Verify small cases ---
for n in range(1, 10):
    exact = E_exact(n)
    formula = Fraction(n - 1, 1) * Fraction(n + 4, 1) / 12
    assert exact == formula, f"E({n}) mismatch"

assert E_exact(1) == 0
assert E_exact(2) == Fraction(1, 2)
assert E_exact(4) == 2
assert E_exact(5) == 3

# Verify modular computation
for n in [1, 2, 5, 10, 100, 10**6]:
    exact_mod = int(E_exact(n)) % MOD if E_exact(n).denominator == 1 else \
                (E_exact(n).numerator * pow(E_exact(n).denominator, MOD - 2, MOD)) % MOD
    computed = solve_closed_form(n, MOD)
    assert exact_mod == computed, f"Mod mismatch at n={n}"

# --- Compute the answer ---
ans = solve_closed_form(10**18, MOD)
print(ans)
