"""
Project Euler Problem 406: Guessing Game

C(n, a, b) = minimum worst-case cost to find a number in {1..n}
with cost a for "too low" and cost b for "too high".

We compute sum of C(10^12, sqrt(k), sqrt(F_k)) for k=1..30.

f(W) = max range searchable with worst-case cost <= W.
f(W) = f(W-a) + f(W-b) + 1 for W >= min(a,b)
f(W) = 1 for 0 <= W < min(a,b)
f(W) = 0 for W < 0

C(n,a,b) = min W such that f(W) >= n.
Binary search on W.
"""

import math
import sys

sys.setrecursionlimit(200000)

def solve(n, a, b):
    """Find minimum W such that f(W) >= n using binary search."""
    lo = 0.0
    hi = math.log2(n) * (a + b) * 2.0

    for _ in range(200):
        mid = (lo + hi) / 2.0
        memo = {}

        def f(i, j):
            budget = mid - i * a - j * b
            if budget < -1e-12:
                return 0

            key = (i, j)
            if key in memo:
                return memo[key]

            cl = budget >= a - 1e-12
            ch = budget >= b - 1e-12

            if not cl and not ch:
                val = 1
            elif cl and ch:
                val = min(10**18, f(i + 1, j) + f(i, j + 1) + 1)
            elif cl:
                val = f(i + 1, j) + 1
            else:
                val = f(i, j + 1) + 1

            memo[key] = val
            return val

        fval = f(0, 0)
        if fval >= n:
            hi = mid
        else:
            lo = mid

    return hi

def main():
    fib = [0] * 31
    fib[1] = 1
    fib[2] = 1
    for i in range(3, 31):
        fib[i] = fib[i - 1] + fib[i - 2]

    N = 10**12

    total = 0.0
    for k in range(1, 31):
        a = math.sqrt(k)
        b = math.sqrt(fib[k])
        c = solve(N, a, b)
        total += c

    print(f"{total:.8f}")

if __name__ == "__main__":
    main()
