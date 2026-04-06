"""
Project Euler 536: Modulo Power Identity

Find S(10^12) where S(n) = sum of all m <= n such that
a^(m+4) = a (mod m) for all integers a.

Key: m must be squarefree and for each prime p | m, (p-1) | (m+3).
We use DFS building m from valid prime factors.
"""

import sys
sys.setrecursionlimit(100000)

LIMIT = 10**12

def is_prime(n):
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def get_divisors(n):
    divs = []
    d = 1
    while d * d <= n:
        if n % d == 0:
            divs.append(d)
            if d != n // d:
                divs.append(n // d)
        d += 1
    divs.sort()
    return divs

def check(m):
    """Check if m satisfies the Korselt-like condition."""
    mp3 = m + 3
    temp = m
    p = 2
    while p * p <= temp:
        if temp % p == 0:
            temp //= p
            if temp % p == 0:
                return False  # not squarefree
            if mp3 % (p - 1) != 0:
                return False
        p += 1
    if temp > 1:
        if mp3 % (temp - 1) != 0:
            return False
    return True

answer = 0

def dfs(m, min_p):
    global answer
    if m > LIMIT:
        return

    if check(m):
        answer += m

    mp3 = m + 3
    divs = get_divisors(mp3)

    for d in divs:
        p = d + 1
        if p < min_p:
            continue
        if p < 2:
            continue
        if m % p == 0:
            continue
        if not is_prime(p):
            continue
        if m * p > LIMIT:
            continue
        dfs(m * p, p + 1)

dfs(1, 2)
print(answer)
