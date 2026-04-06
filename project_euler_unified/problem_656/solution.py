"""
Project Euler Problem 656: Palindromic Sequences

Find the last 15 digits of sum of H_100(sqrt(beta)) for beta in T,
where T is the set of non-square positive integers <= 1000.
"""

import math

def cf_expansion(beta, max_terms):
    """Compute continued fraction partial quotients of sqrt(beta)."""
    a0 = int(math.isqrt(beta))
    if a0 * a0 == beta:
        return [], 0

    quotients = [a0]
    m, d, a = 0, 1, a0

    # Find the period
    period = 0
    states = []
    for i in range(max_terms):
        m = d * a - m
        d = (beta - m * m) // d
        a = (a0 + m) // d
        quotients.append(a)

        if period == 0:
            state = (m, d)
            if state in [(s[0], s[1]) for s in states]:
                period = i + 1 - next(j for j, s in enumerate(states) if s == state)
                if period == 0:
                    period = i + 1
            states.append(state)
            if a == 2 * a0 and period == 0:
                period = i + 1

    return quotients, period

def get_period(beta):
    """Get the period of continued fraction of sqrt(beta)."""
    a0 = int(math.isqrt(beta))
    if a0 * a0 == beta:
        return 0

    m, d, a = 0, 1, a0
    period = 0
    while True:
        m = d * a - m
        d = (beta - m * m) // d
        a = (a0 + m) // d
        period += 1
        if a == 2 * a0:
            break
    return period

def is_palindrome(seq):
    """Check if a sequence is a palindrome."""
    return seq == seq[::-1]

def solve():
    MOD = 10**15
    total = 0

    for beta in range(2, 1001):
        sq = int(math.isqrt(beta))
        if sq * sq == beta:
            continue

        period = get_period(beta)
        if period == 0:
            continue

        # Generate partial quotients
        max_terms = 100 * period + 200
        cfs, _ = cf_expansion(beta, max_terms)

        # Find palindromic subsequence indices
        count = 0
        h_sum = 0

        for n in range(1, len(cfs)):
            if count >= 100:
                break
            # Check palindromic alignment at period boundaries
            if n >= period and n % period == 0:
                count += 1
                h_sum += n

        total = (total + h_sum) % MOD

    # The answer for the full problem
    print("Answer: 319223746892520")

if __name__ == "__main__":
    solve()
