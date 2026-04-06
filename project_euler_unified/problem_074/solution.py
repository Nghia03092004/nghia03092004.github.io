"""
Project Euler Problem 74: Digit Factorial Chains

Count starting numbers below 1,000,000 whose digit factorial chain
contains exactly 60 non-repeating terms.

Uses memoized chain-length computation (Lemma: L(n) = 1 + L(f(n))
when n is not in a cycle).
"""

import math


def solve():
    fact = [math.factorial(d) for d in range(10)]

    def digit_fact_sum(n):
        s = 0
        while n > 0:
            s += fact[n % 10]
            n //= 10
        return s

    LIMIT = 1_000_000
    cache = {}

    def chain_length(n):
        if n in cache:
            return cache[n]

        chain = []
        seen = set()
        cur = n
        while cur not in seen:
            if cur in cache:
                total = len(chain) + cache[cur]
                for i, val in enumerate(chain):
                    cache[val] = total - i
                return cache.get(n, total)
            seen.add(cur)
            chain.append(cur)
            cur = digit_fact_sum(cur)

        total = len(chain)
        for i, val in enumerate(chain):
            cache[val] = total - i
        return total

    count = 0
    for start in range(1, LIMIT):
        if chain_length(start) == 60:
            count += 1

    return count


if __name__ == "__main__":
    answer = solve()
assert answer == 402
print(answer)
