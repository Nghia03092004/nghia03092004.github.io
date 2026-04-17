"""
Project Euler Problem 528: Constrained Sums

S(n,k,b) = number of solutions to x1+x2+...+xk <= n with 0 <= xm <= b^m.
Find (sum_{k=10}^{15} S(10^k, k, k)) mod 10^9+7.

Uses inclusion-exclusion with modular arithmetic.
"""

MOD = 10**9 + 7

def mod_inv(a, m=MOD):
    return pow(a, m - 2, m)

def binom_large_n(N, k):
    """Compute C(N, k) mod MOD where N can be very large but k is small."""
    if N < 0 or N < k:
        return 0
    if k == 0:
        return 1

    num = 1
    for i in range(k):
        num = num * ((N - i) % MOD) % MOD

    den = 1
    for i in range(1, k + 1):
        den = den * i % MOD

    return num * mod_inv(den) % MOD

def compute_S(n, k, b):
    """Compute S(n, k, b) mod MOD using inclusion-exclusion."""
    # Precompute b^m for m = 1..k
    bpow = [1] * (k + 1)
    for m in range(1, k + 1):
        bpow[m] = bpow[m-1] * b

    result = 0

    # Enumerate subsets of {1,...,k}
    for mask in range(1 << k):
        bits = bin(mask).count('1')
        subtract = 0
        valid = True

        for m in range(k):
            if mask & (1 << m):
                subtract += bpow[m + 1] + 1
                if subtract > n:
                    valid = False
                    break

        if not valid:
            continue

        remaining = n - subtract
        coeff = binom_large_n(remaining + k, k)

        if bits % 2 == 0:
            result = (result + coeff) % MOD
        else:
            result = (result - coeff + MOD) % MOD

    return result

# Verify small cases
print(f"S(14,3,2) = {compute_S(14, 3, 2)}")      # 135
print(f"S(200,5,3) = {compute_S(200, 5, 3)}")    # 12949440

total = 0
for k in range(10, 16):
    n = 10**k
    s = compute_S(n, k, k)
    print(f"S(10^{k},{k},{k}) mod 10^9+7 = {s}")
    total = (total + s) % MOD

print(f"Answer: {total}")
