"""
Problem 303: Multiples with Small Digits

For each n from 1 to 10000, find f(n) = smallest positive multiple of n
using only digits {0, 1, 2}. Compute sum of f(n)/n.

BFS on remainders mod n.
"""

from collections import deque

def find_f_over_n(n):
    """Find f(n)/n using BFS on remainders."""
    # BFS: build numbers digit by digit, track remainder mod n
    visited = [False] * n
    # parent[r] = (prev_remainder, digit) or None for root
    parent = [None] * n

    q = deque()
    for d in (1, 2):
        r = d % n
        if r == 0:
            return d  # f(n) = d
        if not visited[r]:
            visited[r] = True
            parent[r] = (-1, d)  # -1 means root
            q.append(r)

    while q:
        r = q.popleft()
        for d in (0, 1, 2):
            nr = (10 * r + d) % n
            if nr == 0:
                # Reconstruct digits
                digits = [d]
                cur = r
                while parent[cur][0] != -1:
                    digits.append(parent[cur][1])
                    cur = parent[cur][0]
                digits.append(parent[cur][1])
                digits.reverse()
                # Build f(n)
                fn = 0
                for dig in digits:
                    fn = fn * 10 + dig
                return fn // n
            if not visited[nr]:
                visited[nr] = True
                parent[nr] = (r, d)
                q.append(nr)

    return 0  # Should never reach here

def solve():
    total = 0
    for n in range(1, 10001):
        total += find_f_over_n(n)
    print(total)

solve()
