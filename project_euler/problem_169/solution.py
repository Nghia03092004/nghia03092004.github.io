"""
Problem 169: Number of ways to express n as sum of powers of 2,
each power used at most twice. Find f(10^25).

Recurrence:
  f(0) = 1
  f(n) = f(n//2)              if n is odd
  f(n) = f(n//2) + f(n//2-1)  if n is even
"""

import sys
sys.setrecursionlimit(10000)

memo = {}

def f(n):
    if n == 0:
        return 1
    if n in memo:
        return memo[n]
    if n % 2 == 1:
        result = f(n // 2)
    else:
        result = f(n // 2) + f(n // 2 - 1)
    memo[n] = result
    return result

print(f(10**25))
