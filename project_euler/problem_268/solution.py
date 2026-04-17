"""
Problem 268: Counting numbers with at least four distinct prime factors < 100

Count numbers below 10^16 divisible by at least 4 distinct primes less than 100.

Approach: Inclusion-exclusion.
For r = 4, the count of numbers with at least r distinct prime factors from a set is:
  sum_{k=r}^{25} (-1)^{k-r} * C(k-1, r-1) * S_k
where S_k = sum over all k-subsets of primes of floor(M / product).
"""

from math import comb
from itertools import combinations

def solve():
    M = 10**16 - 1  # numbers below 10^16 means [1, 10^16 - 1]

    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47,
              53, 59, 61, 67, 71, 73, 79, 83, 89, 97]

    # Recursive enumeration with pruning
    answer = 0

    def dfs(idx, cnt, prod):
        nonlocal answer
        if cnt >= 4:
            term = M // prod
            sign = 1 if (cnt - 4) % 2 == 0 else -1
            binom = comb(cnt - 1, 3)
            answer += sign * binom * term

        for i in range(idx, len(primes)):
            if prod * primes[i] > M:
                break
            dfs(i + 1, cnt + 1, prod * primes[i])

    dfs(0, 0, 1)
    print(answer)

solve()
