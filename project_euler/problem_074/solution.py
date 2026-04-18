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
        if n == 0:
            return fact[0]
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
        position = {}
        cur = n
        while cur not in position and cur not in cache:
            position[cur] = len(chain)
            chain.append(cur)
            cur = digit_fact_sum(cur)

        if cur in cache:
            length = cache[cur]
            for val in reversed(chain):
                length += 1
                cache[val] = length
            return cache[n]

        cycle_start = position[cur]
        cycle_length = len(chain) - cycle_start

        for i in range(cycle_start, len(chain)):
            cache[chain[i]] = cycle_length

        length = cycle_length
        for i in range(cycle_start - 1, -1, -1):
            length += 1
            cache[chain[i]] = length

        return cache[n]

    count = 0
    for start in range(1, LIMIT):
        if chain_length(start) == 60:
            count += 1

    return count


if __name__ == "__main__":
    answer = solve()
    assert answer == 402
    print(answer)
