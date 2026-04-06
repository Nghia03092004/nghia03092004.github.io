"""
Problem 705: Total Inversion Count of Digit Strings
"""

def solve():
    MOD = 10**9 + 7
    n = 10**16
    # T(n) = 45 * n * (n-1) / 2 * 10^(n-2)
    n_mod = n % MOD
    nm1 = (n - 1) % MOD
    inv2 = pow(2, MOD - 2, MOD)
    power10 = pow(10, n - 2, MOD)
    result = 45 * n_mod % MOD * nm1 % MOD * inv2 % MOD * power10 % MOD
    return result

answer = solve()
print(answer)
