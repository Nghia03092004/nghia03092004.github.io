"""
Problem 323: Bitwise-OR Operations on Random Integers

E[N] = sum_{j=1}^{32} (-1)^{j+1} * C(32,j) / (1 - 2^{-j})

where N is the first time all 32 bits are set after OR-ing with random 32-bit integers.
"""
from math import comb
from decimal import Decimal, getcontext

def solve():
    getcontext().prec = 50

    result = Decimal(0)
    for j in range(1, 33):
        sign = Decimal(1) if j % 2 == 1 else Decimal(-1)
        c = Decimal(comb(32, j))
        denom = 1 - Decimal(2) ** (-j)
        result += sign * c / denom

    # Round to 10 decimal places
    print(f"{result:.10f}")

if __name__ == "__main__":
    solve()
