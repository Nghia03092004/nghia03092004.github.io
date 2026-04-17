"""
Problem 581: 47-smooth Triangular Numbers

T(n) = n(n+1)/2 is 47-smooth iff both n and n+1 are 47-smooth (Theorem 1).
By Stormer's theorem, there are finitely many such n, all below 1.2e12.

Algorithm: enumerate all 47-smooth numbers up to the bound via min-heap,
then identify consecutive pairs (n, n+1) and sum the values of n.

Answer: 2227616372734
"""

import heapq

def solve():
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47]
    LIMIT = 1_200_000_000_000

    heap = [1]
    visited = {1}
    prev = -1
    total = 0

    while heap:
        v = heapq.heappop(heap)
        if v > LIMIT:
            break
        if v == prev + 1 and prev >= 1:
            total += prev
        prev = v
        for p in primes:
            w = v * p
            if w <= LIMIT and w not in visited:
                visited.add(w)
                heapq.heappush(heap, w)

    print(total)

solve()
