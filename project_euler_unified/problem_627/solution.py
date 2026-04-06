"""
Problem 627: Counting Products

f(n) = number of ordered factorizations of n into factors >= 2.
F(N) = sum_{n=2}^{N} f(n).

Dirichlet series: sum f(n)/n^s = 1/(2 - zeta(s)).
Recurrence: f(n) = sum_{d|n, d<n} f(d), f(1) = 1.

Method 1: Sieve-like DP in O(N log N)
Method 2: Recursive with memoization (verification)
"""

# --- Method 1: Sieve DP ---
def solve_sieve(N: int) -> list:
    """Compute f(n) for n=1..N using sieve-like DP."""
    f = [0] * (N + 1)
    f[1] = 1
    for d in range(1, N + 1):
        for m in range(2 * d, N + 1, d):
            f[m] += f[d]
    return f

# --- Method 2: Recursive ---
def solve_recursive(N: int) -> list:
    """Compute f(n) recursively."""
    f = [0] * (N + 1)
    f[1] = 1
    for n in range(2, N + 1):
        total = 0
        d = 1
        while d * d <= n:
            if n % d == 0:
                if d < n:
                    total += f[d]
                if d != n // d and n // d < n:
                    total += f[n // d]
            d += 1
        f[n] = total
    return f

# Verify
N = 1000
f_sieve = solve_sieve(N)
f_rec = solve_recursive(N)

for n in range(1, N + 1):
    assert f_sieve[n] == f_rec[n], f"Mismatch at n={n}"

# Known values
assert f_sieve[1] == 1
assert f_sieve[2] == 1
assert f_sieve[4] == 2
assert f_sieve[6] == 3
assert f_sieve[8] == 4
assert f_sieve[12] == 8
assert f_sieve[16] == 8

# f(p^k) = 2^{k-1}
for k in range(1, 10):
    assert f_sieve[2**k] == 2**(k-1), f"f(2^{k}) = {f_sieve[2**k]}, expected {2**(k-1)}"

F_N = sum(f_sieve[2:])
print(f"F({N}) = {F_N}")
print("All verifications passed.")
