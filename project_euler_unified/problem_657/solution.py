"""
Project Euler Problem 657: Incomplete Words

Find I(10^7, 10^12) mod 10^9+7.

I(alpha, n) = sum_{j=1}^{alpha} (-1)^{j+1} * C(alpha,j) * geo(alpha-j, n)
where geo(i, n) = (i^{n+1} - 1) / (i - 1)
"""

MOD = 1000000007

def power(base, exp, mod):
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = result * base % mod
        base = base * base % mod
        exp >>= 1
    return result

def mod_inv(a, mod):
    return power(a, mod - 2, mod)

def solve():
    alpha = 10**7
    n = 10**12

    ans = 0
    binom = 1  # C(alpha, j) mod MOD, built iteratively

    for j in range(1, alpha + 1):
        # C(alpha, j) = C(alpha, j-1) * (alpha - j + 1) / j
        binom = binom * ((alpha - j + 1) % MOD) % MOD
        binom = binom * mod_inv(j, MOD) % MOD

        i = alpha - j  # base of geometric series

        if i == 0:
            geo = 1
        elif i == 1:
            geo = (n + 1) % MOD
        else:
            num = (power(i, n + 1, MOD) - 1 + MOD) % MOD
            den = (i % MOD - 1 + MOD) % MOD
            geo = num * mod_inv(den, MOD) % MOD

        term = binom * geo % MOD

        if j % 2 == 1:
            ans = (ans + term) % MOD
        else:
            ans = (ans - term + MOD) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
