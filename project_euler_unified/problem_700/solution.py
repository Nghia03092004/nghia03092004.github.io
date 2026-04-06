#!/usr/bin/env python3
"""Project Euler Problem 700: Eulercoin"""

def solve():
    N = 1504170715041707
    M = 4503599627370517
    N_inv = pow(N, -1, M)

    total = 0
    min_coin = M

    # Phase 1: brute force forward
    val = 0
    for i in range(1, 200000000):
        val = (val + N) % M
        if val < min_coin:
            min_coin = val
            total += val
            if min_coin < 10000000:
                break

    # Phase 2: backward search from small values
    max_index = M
    for v in range(1, min_coin):
        nv = v * N_inv % M
        if nv < max_index:
            max_index = nv
            total += v

    print(total)

solve()
