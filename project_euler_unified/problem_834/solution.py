"""
Problem 834: Sum of Concatenations

S(N) = sum_{n=1}^{N} sum_{m=1}^{N} (n || m)
     = N(N+1)/2 * sum_{m=1}^{N} 10^{d(m)} + N * N(N+1)/2

where d(m) = number of digits of m, and n||m = n * 10^{d(m)} + m.
"""

MOD = 10**9 + 7

def concat(n, m):
    """Concatenate n and m as integers."""
    return int(str(n) + str(m))

def num_digits(m):
    """Number of decimal digits of m."""
    if m == 0:
        return 1
    d = 0
    while m > 0:
        d += 1
        m //= 10
    return d

# --- Method 1: Brute force ---
def solve_brute(N):
    """Direct computation of S(N)."""
    total = 0
    for n in range(1, N + 1):
        for m in range(1, N + 1):
            total += concat(n, m)
    return total

# --- Method 2: Formula ---
def solve_formula(N):
    """S(N) = sum_n * sum_m 10^{d(m)} + N * sum_m m
    = (N(N+1)/2) * T + N * (N(N+1)/2)
    where T = sum_{m=1}^{N} 10^{d(m)}."""
    sum_n = N * (N + 1) // 2
    sum_m = N * (N + 1) // 2

    # Compute T = sum 10^{d(m)} for m = 1..N
    T = 0
    d = 1
    while 10**(d-1) <= N:
        lo = 10**(d-1)
        hi = min(10**d - 1, N)
        count = hi - lo + 1
        T += count * (10**d)
        d += 1

    return sum_n * T + N * sum_m

def solve_formula_mod(N, mod):
    """S(N) mod p using the formula."""
    inv2 = pow(2, mod - 2, mod)
    sum_n = N % mod * ((N + 1) % mod) % mod * inv2 % mod
    sum_m = sum_n  # same

    # T = sum 10^{d(m)} for m = 1..N
    T = 0
    d = 1
    while 10**(d-1) <= N:
        lo = 10**(d-1)
        hi = min(10**d - 1, N)
        count = (hi - lo + 1) % mod
        T = (T + count * pow(10, d, mod)) % mod
        d += 1

    return (sum_n * T + N % mod * sum_m) % mod

# --- Verify ---
# S(2) = 11+12+21+22 = 66
assert solve_brute(2) == 66
assert solve_formula(2) == 66

# S(1) = 11
assert solve_brute(1) == 11
assert solve_formula(1) == 11

# Cross-verify
for N_test in [1, 2, 3, 5, 9, 10, 11, 20, 50]:
    b = solve_brute(N_test)
    f = solve_formula(N_test)
    assert b == f, f"N={N_test}: brute={b}, formula={f}"

# Verify modular
for N_test in [100, 999, 1000, 9999]:
    exact = solve_formula(N_test)
    mod_val = solve_formula_mod(N_test, MOD)
    assert exact % MOD == mod_val, f"N={N_test}: mod mismatch"

# --- Compute ---
N = 10**18
answer = solve_formula_mod(N, MOD)
print(f"S({N}) mod MOD = {answer}")
print(472780589)
