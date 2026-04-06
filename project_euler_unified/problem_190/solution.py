"""
Problem 190: Maximising a Weighted Product

For m from 2 to 15, find the maximum of prod(x_i^i) subject to sum(x_i) = m,
then sum floor of each maximum.
"""
import math
from decimal import Decimal, getcontext

def solve():
    getcontext().prec = 50
    total = 0

    for m in range(2, 16):
        # Optimal: x_i = 2i / (m+1)
        # P_m = product of (2i/(m+1))^i
        log_pm = Decimal(0)
        for i in range(1, m + 1):
            xi = Decimal(2 * i) / Decimal(m + 1)
            log_pm += i * xi.ln()

        pm = log_pm.exp()
        total += int(pm)

    print(total)

if __name__ == "__main__":
    solve()
