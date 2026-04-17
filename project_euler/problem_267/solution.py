"""
Problem 267: Billionaire

Toss a fair coin 1000 times. Before each toss, bet a fixed fraction f of
current wealth. Heads: gain 2f*wealth (multiply by 1+2f). Tails: lose
f*wealth (multiply by 1-f).

Find the maximum probability of reaching $10^9 starting from $1.

Approach:
- After k heads out of 1000: wealth = (1+2f)^k * (1-f)^(1000-k)
- For fixed k, optimal f = (3k - 1000) / 2000
- Find minimum k where max wealth >= 10^9
- Probability = P(Binomial(1000, 0.5) >= k_min)
"""

from math import log, lgamma, exp, ceil
from decimal import Decimal, getcontext

def solve():
    n = 1000
    target = 9 * log(10)

    # Find minimum k such that at optimal f, wealth >= 10^9
    kmin = None
    for k in range(n // 3 + 1, n + 1):
        f_opt = (3 * k - n) / (2 * n)
        if f_opt <= 0 or f_opt >= 1:
            continue
        log_wealth = k * log(1 + 2 * f_opt) + (n - k) * log(1 - f_opt)
        if log_wealth >= target - 1e-12:
            kmin = k
            break

    if kmin is None:
        print("0.000000000000")
        return

    # Use Decimal for high-precision computation
    getcontext().prec = 50
    prob = Decimal(0)
    two = Decimal(2)
    two_n = two ** n  # 2^1000

    # Compute sum_{k=kmin}^{n} C(n,k) / 2^n using exact integer arithmetic
    from math import comb
    total = sum(comb(n, k) for k in range(kmin, n + 1))
    prob = Decimal(total) / Decimal(two_n)

    # Format to 12 decimal places
    print(f"{prob:.12f}")

solve()
