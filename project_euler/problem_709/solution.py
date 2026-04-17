"""
Problem 709: Even Stevens
"""

def solve():
    MOD = 10**9 + 7
    N = 10**7
    inv2 = pow(2, MOD - 2, MOD)
    inv3 = pow(3, MOD - 2, MOD)

    # p(n) = 2/3 + (1/3)(-1/2)^n
    # sum_{n=1}^{N} p(n) = 2N/3 + (1/3) * sum_{n=1}^{N} (-1/2)^n
    # Geometric sum: sum = (-1/2)(1 - (-1/2)^N) / (1 - (-1/2)) = (-1/2)(1-(-1/2)^N)/(3/2)
    # = -(1-(-1/2)^N)/3

    two_N_over_3 = 2 * N % MOD * inv3 % MOD

    # (-1/2)^N mod MOD
    neg_half = MOD - inv2  # -1/2 mod MOD
    neg_half_N = pow(neg_half, N, MOD)

    geo_sum = (MOD - 1) * (1 - neg_half_N + MOD) % MOD * inv3 % MOD

    result = (two_N_over_3 + inv3 * geo_sum) % MOD
    return result

answer = solve()
print(answer)
