#!/usr/bin/env python3
"""
Project Euler Problem 757: Stealthy Numbers
A number N is stealthy if N = x(x+1)*y(y+1) for positive integers x, y.
Count stealthy numbers <= 10^14.
"""

def solve():
    LIMIT = 10**14
    results = set()

    x = 1
    while x * (x + 1) * x * (x + 1) <= LIMIT:
        px = x * (x + 1)
        y = x
        while True:
            py = y * (y + 1)
            val = px * py
            if val > LIMIT:
                break
            results.add(val)
            y += 1
        x += 1

    print(len(results))

solve()
