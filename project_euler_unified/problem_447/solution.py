"""
Problem 447: Retractions B

The number of retractions R(n) for affine maps f(x) = ax + b mod n equals
the number of pairs (a, b) with a^2 ≡ a (mod n) and ab ≡ 0 (mod n).

Key result: R(n) = prod_{p^e || n} (1 + p^e), a multiplicative function.

We compute sum_{n=2}^{N} R(n) mod (10^9 + 7) using a linear sieve.
"""

from math import gcd

MOD = 10**9 + 7

# --- Method 1: Linear sieve using smallest prime factor ---
def solve_linear_sieve(N: int, mod: int):
    """Compute R(n) for n=1..N via linear sieve. Return (R_array, total_sum)."""
    spf = list(range(N + 1))  # smallest prime factor
    for i in range(2, int(N**0.5) + 1):
        if spf[i] == i:  # i is prime
            for j in range(i * i, N + 1, i):
                if spf[j] == j:
                    spf[j] = i

    R = [0] * (N + 1)
    R[1] = 1  # R(1) = 1 (only a=0, b=0 trivially, but by convention)
    total = 0

    for n in range(2, N + 1):
        p = spf[n]
        # Find p^e exactly dividing n
        e = 0
        m = n
        while m % p == 0:
            m //= p
            e += 1
        # R(n) = R(m) * (1 + p^e)
        pe = p ** e
        R[n] = R[m] * ((1 + pe) % mod) % mod
        total = (total + R[n]) % mod

    return R, total

# --- Method 2: Brute force for small n (verification) ---
def R_brute(n: int):
    """Count retractions by brute force: pairs (a, b) with a^2≡a, ab≡0 mod n."""
    count = 0
    for a in range(n):
        if (a * a) % n != a % n:
            continue
        for b in range(n):
            if (a * b) % n == 0:
                count += 1
    return count

# --- Method 3: Product formula (verification) ---
def R_product(n: int):
    """Compute R(n) = prod_{p^e || n} (1 + p^e)."""
    if n <= 1:
        return 1
    result = 1
    d = 2
    temp = n
    while d * d <= temp:
        if temp % d == 0:
            pe = 1
            while temp % d == 0:
                pe *= d
                temp //= d
            result *= (1 + pe)
        d += 1
    if temp > 1:
        result *= (1 + temp)
    return result

# --- Compute answer ---
N = 10**7
R_arr, ans = solve_linear_sieve(N, MOD)

# --- Verify against brute force for small n ---
for n in range(2, 50):
    bf = R_brute(n)
    pf = R_product(n)
    assert bf == pf, f"Brute vs product mismatch at n={n}: {bf} vs {pf}"
    assert bf % MOD == R_arr[n], f"Sieve mismatch at n={n}: {R_arr[n]} vs {bf}"

# --- Verify product formula examples ---
assert R_product(2) == 3
assert R_product(6) == 12
assert R_product(12) == 20
assert R_product(30) == 72

print(ans)
