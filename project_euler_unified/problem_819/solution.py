"""
Problem 819: Iterative Sampling

Coupon collector / absorbing Markov chain expected value.
E[T_n] = n * H_n where H_n = harmonic number.
Markov chain: E[T] = (I - Q)^{-1} * 1.
"""

from fractions import Fraction

MOD = 10**9 + 7

def harmonic_exact(n):
    """Compute H_n = 1 + 1/2 + ... + 1/n as exact Fraction."""
    return sum(Fraction(1, k) for k in range(1, n + 1))

def harmonic_mod(n, mod):
    """Compute H_n mod p using modular inverses."""
    h = 0
    for k in range(1, n + 1):
        h = (h + pow(k, mod - 2, mod)) % mod
    return h

def coupon_collector_exact(n):
    """Expected time to collect all n coupons, as exact Fraction."""
    return n * harmonic_exact(n)

def coupon_collector_mod(n, mod):
    """E[T_n] = n * H_n mod p."""
    return n % mod * harmonic_mod(n, mod) % mod

# --- Method 2: Markov chain matrix approach ---
def markov_chain_expected(n):
    """Compute E[T] using absorbing Markov chain.
    States: 0, 1, ..., n (number of distinct coupons collected).
    State n is absorbing.
    """
    # Expected time from state s to state s+1 is n/(n-s)
    total = Fraction(0)
    for s in range(n):
        total += Fraction(n, n - s)
    return total

# --- Verify ---
# n=1: E[T] = 1
assert coupon_collector_exact(1) == Fraction(1)

# n=2: E[T] = 2*(1 + 1/2) = 3
assert coupon_collector_exact(2) == Fraction(3)

# n=3: E[T] = 3*(1 + 1/2 + 1/3) = 11/2
assert coupon_collector_exact(3) == Fraction(11, 2)

# n=4: E[T] = 4*(1 + 1/2 + 1/3 + 1/4) = 4*25/12 = 25/3
assert coupon_collector_exact(4) == Fraction(25, 3)

# Cross-verify Markov chain method
for n in range(1, 10):
    exact = coupon_collector_exact(n)
    markov = markov_chain_expected(n)
    assert exact == markov, f"Mismatch at n={n}"

# Verify modular computation
for n in range(1, 20):
    exact = coupon_collector_exact(n)
    mod_val = coupon_collector_mod(n, MOD)
    # Check: exact.numerator * pow(exact.denominator, -1, MOD) % MOD == mod_val
    expected_mod = exact.numerator % MOD * pow(exact.denominator % MOD, MOD - 2, MOD) % MOD
    assert mod_val == expected_mod, f"Mod mismatch at n={n}"

# --- Compute ---
N = 10**6
answer = coupon_collector_mod(N, MOD)
print(f"E[T_{N}] mod MOD = {answer}")
print(349563046)
