"""
Project Euler 638: Weighted Lattice Paths

C(a,b,k) = Gaussian binomial coefficient [a+b choose a]_k
         = prod_{i=1}^{min(a,b)} (k^{a+b-i+1} - 1) / (k^i - 1) for k >= 2
         = binom(a+b, a) for k = 1

Find sum_{k=1}^{7} C(10^k+k, 10^k+k, k) mod 10^9+7.
"""

MOD = 10**9 + 7

def power(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = result * base % mod
        base = base * base % mod
        exp >>= 1
    return result

def modinv(a, mod):
    return power(a, mod - 2, mod)

def compute_C(n, k):
    """Compute C(n, n, k) = [2n choose n]_k mod MOD."""
    if k == 1:
        # binom(2n, n) mod MOD
        result = 1
        for i in range(1, n + 1):
            result = result * ((n + i) % MOD) % MOD
            result = result * modinv(i % MOD, MOD) % MOD
        return result

    # For k >= 2: prod_{i=1}^{n} (k^{n+i} - 1) / (k^i - 1)
    result = 1
    for i in range(1, n + 1):
        exp_num = (n + i) % (MOD - 1)
        exp_den = i % (MOD - 1)

        num = (power(k, exp_num, MOD) - 1) % MOD
        den = (power(k, exp_den, MOD) - 1) % MOD

        result = result * num % MOD
        result = result * modinv(den, MOD) % MOD

    return result

def solve():
    total = 0
    for k in range(1, 8):
        n = 10**k + k
        c = compute_C(n, k)
        total = (total + c) % MOD
        print(f"k={k}, n={n}, C={c}")

    print(total)

if __name__ == "__main__":
    solve()
