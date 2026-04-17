"""
Problem 63: Powerful Digit Counts

How many n-digit positive integers exist which are also an nth power?
"""

import math


def solve():
    count = 0
    for a in range(1, 10):
        if a == 1:
            count += 1
        else:
            # N(a) = floor(1 / (1 - log10(a)))
            count += int(math.floor(1.0 / (1.0 - math.log10(a))))
    return count


# Verification by brute force
def solve_brute():
    count = 0
    for a in range(1, 10):
        for n in range(1, 100):
            if len(str(a ** n)) == n:
                count += 1
    return count


answer = solve()
assert answer == solve_brute(), f"Mismatch: {answer} vs {solve_brute()}"
print(answer)
