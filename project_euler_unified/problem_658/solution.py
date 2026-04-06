"""
Project Euler Problem 658: Incomplete Words II

Find S(10^7, 10^12) mod 10^9+7, where S(k,n) = sum_{alpha=1}^{k} I(alpha, n).

Using the derived formula:
S(k,n) = sum_{m=1}^{k} (-1)^{k-m+2} * C(k,m) * G(m-1, n)
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
    return power(a % mod, mod - 2, mod)

def solve():
    k = 10**7
    n = 10**12

    ans = 0
    binom = 1

    for m in range(1, k + 1):
        binom = binom * ((k - m + 1) % MOD) % MOD
        binom = binom * mod_inv(m, MOD) % MOD

        i = m - 1

        if i == 0:
            geo = 1
        elif i == 1:
            geo = (n + 1) % MOD
        else:
            num = (power(i, n + 1, MOD) - 1 + MOD) % MOD
            den = (i - 1) % MOD
            geo = num * mod_inv(den, MOD) % MOD

        term = binom * geo % MOD

        sign_exp = k - m + 2
        if sign_exp % 2 == 0:
            ans = (ans + term) % MOD
        else:
            ans = (ans - term + MOD) % MOD

    print(ans)

if __name__ == "__main__":
    solve()
