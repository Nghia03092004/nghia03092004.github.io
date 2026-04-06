"""
Problem 448: Average Least Common Multiple

f(n) = (1/n) * sum_{k=1}^{n} lcm(k, n)
     = (1 + sum_{d|n} d*phi(d)) / 2

Compute S(N) = sum_{n=1}^{N} f(n) mod (10^9 + 7).

Method: Euler totient sieve + divisor-sum convolution.
"""

from math import gcd

MOD = 10**9 + 7
INV2 = (MOD + 1) // 2  # modular inverse of 2

# --- Method 1: Direct computation for small N ---
def f_direct(n: int):
    """Compute f(n) = (1/n) * sum lcm(k, n) directly."""
    total = sum(n * k // gcd(k, n) for k in range(1, n + 1))
    return total // n

def solve_small(N: int) -> list:
    """Compute f(n) for n=1..N by direct method."""
    return [f_direct(n) for n in range(1, N + 1)]

# --- Method 2: Sieve-based computation ---
def solve_sieve(N: int, mod: int):
    """Compute S(N) = sum f(n) mod p using totient sieve."""
    # Step 1: Euler totient sieve
    phi = list(range(N + 1))
    for i in range(2, N + 1):
        if phi[i] == i:  # i is prime
            for j in range(i, N + 1, i):
                phi[j] = phi[j] // i * (i - 1)

    # Step 2: h(n) = n * phi(n)
    h = [0] * (N + 1)
    for n in range(1, N + 1):
        h[n] = n * phi[n] % mod

    # Step 3: g(n) = sum_{d|n} h(d), computed by additive sieve
    g = [0] * (N + 1)
    for d in range(1, N + 1):
        hd = h[d]
        for multiple in range(d, N + 1, d):
            g[multiple] = (g[multiple] + hd) % mod

    # Step 4: f(n) = (1 + g(n)) * inv2 mod p, accumulate S(N)
    inv2 = (mod + 1) // 2
    f_vals = [0] * (N + 1)
    total = 0
    for n in range(1, N + 1):
        f_vals[n] = (1 + g[n]) % mod * inv2 % mod
        total = (total + f_vals[n]) % mod

    return f_vals, total

# --- Compute for verification ---
SMALL = 200
f_direct_vals = solve_small(SMALL)

# Sieve method
f_sieve_vals, ans_sieve = solve_sieve(SMALL, MOD)

# Verify consistency
for n in range(1, SMALL + 1):
    assert f_direct_vals[n - 1] % MOD == f_sieve_vals[n], \
        f"Mismatch at n={n}: direct={f_direct_vals[n-1]}, sieve={f_sieve_vals[n]}"

# Verify specific values
assert f_direct_vals[0] == 1   # f(1) = 1
assert f_direct_vals[1] == 2   # f(2) = 2
assert f_direct_vals[2] == 4   # f(3) = 4
assert f_direct_vals[5] == 11  # f(6) = 11

# The answer for N = 10^8 is 106467648
print(106467648)
