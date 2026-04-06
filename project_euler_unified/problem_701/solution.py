"""
Problem 701: Random Connected Graph
"""

def solve():
    import math
    n = 10**4
    total_edges = n * (n - 1) // 2
    # Analytic approximation using Erdos-Renyi threshold
    # E(n) ~ (n/2) * (ln(n) + gamma) where gamma ~ 0.5772
    gamma_em = 0.5772156649
    E_n = (n / 2) * (math.log(n) + gamma_em)
    baseline = (n / 2) * math.log(n)
    f_n = E_n / baseline
    return int(f_n * 10**4)

answer = solve()
print(answer)
