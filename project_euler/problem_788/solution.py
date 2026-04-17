"""
Problem 788: Dominating Numbers

Count positive integers with at most N digits for which some digit appears
more than half the time.
"""

MOD = 1_000_000_007
N = 2022


def mod_pow(base, exp, mod=MOD):
    result = 1
    base %= mod
    while exp > 0:
        if exp & 1:
            result = result * base % mod
        base = base * base % mod
        exp >>= 1
    return result


def solve():
    fact = [1] * (N + 1)
    for i in range(1, N + 1):
        fact[i] = fact[i - 1] * i % MOD

    inv_fact = [1] * (N + 1)
    inv_fact[N] = mod_pow(fact[N], MOD - 2)
    for i in range(N, 0, -1):
        inv_fact[i - 1] = inv_fact[i] * i % MOD

    def comb(n, k):
        if k < 0 or k > n:
            return 0
        return fact[n] * inv_fact[k] % MOD * inv_fact[n - k] % MOD

    pow9 = [1] * (N + 1)
    for i in range(1, N + 1):
        pow9[i] = pow9[i - 1] * 9 % MOD

    total = 0
    for n in range(1, N + 1):
        subtotal = 0
        for k in range(n // 2 + 1, n + 1):
            subtotal = (subtotal + comb(n, k) * pow9[n - k]) % MOD
        total = (total + subtotal) % MOD
    return total * 9 % MOD


if __name__ == "__main__":
    answer = solve()
    print(answer)
