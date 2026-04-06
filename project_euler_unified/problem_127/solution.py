"""
Problem 127: abc-hits

Find the sum of c for all abc-hits below 120000.
An abc-hit: gcd(a,b)=1, a<b, a+b=c, rad(abc) < c.
"""

from math import gcd

def solve():
    N = 120000
    rad = [1] * N

    # Sieve for radicals
    for p in range(2, N):
        if rad[p] == 1:  # p is prime
            for m in range(p, N, p):
                rad[m] *= p

    # Sort indices 1..N-1 by their radical (skip 0)
    sorted_by_rad = sorted(range(1, N), key=lambda x: rad[x])

    total = 0

    for c in range(3, N):
        threshold = c // rad[c]
        if threshold <= 1:
            continue

        for a in sorted_by_rad:
            if rad[a] >= threshold:
                break
            if a >= c:
                continue
            b = c - a
            if a >= b:
                continue
            if rad[a] * rad[b] >= threshold:
                continue
            if gcd(rad[a], rad[b]) != 1:
                continue
            total += c

    return total

if __name__ == '__main__':
    answer = solve()
    print(answer)
