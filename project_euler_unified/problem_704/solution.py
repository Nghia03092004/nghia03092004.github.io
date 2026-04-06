#!/usr/bin/env python3
"""Project Euler Problem 704: Factors of Two in Binomial Coefficients"""

def solve():
    N = 10**16

    # F(n) = floor(log2(n)) - v2(n) where v2(n) is 2-adic valuation
    # S(N) = sum F(n) for n=1..N = A(N) - B(N)

    # A(N) = sum of floor(log2(n)) for n=1..N
    B = N.bit_length() - 1  # floor(log2(N))
    powerB = 1 << B
    # sum_{b=0}^{B-1} b * 2^b = (B-2)*2^B + 2
    A = (B - 2) * powerB + 2 + B * (N - powerB + 1)

    # B(N) = sum of v2(n) for n=1..N = sum_{k>=1} floor(N/2^k)
    Bval = 0
    for k in range(1, B + 1):
        Bval += N >> k

    result = A - Bval
    print(result)

    # Verify with small cases
    def F(n):
        if n == 0:
            return 0
        return (n.bit_length() - 1) - (n & -n).bit_length() + 1

    def S_brute(N):
        return sum(F(n) for n in range(1, N + 1))

    # Check S(100)
    print(f"S(100) = {S_brute(100)} (expected 389)")
    print(f"S(10^7) check with formula:")

    # Formula check for N=100
    B100 = 99 .bit_length() - 1  # Actually need bit_length of 100
    B100 = (100).bit_length() - 1
    p100 = 1 << B100
    A100 = (B100 - 2) * p100 + 2 + B100 * (100 - p100 + 1)
    Bval100 = sum(100 >> k for k in range(1, B100 + 1))
    print(f"Formula S(100) = {A100 - Bval100} (expected 389)")

solve()
