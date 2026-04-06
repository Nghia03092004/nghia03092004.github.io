"""
Problem 47: Distinct Primes Factors

Find the first four consecutive integers each having exactly four
distinct prime factors.
"""

def solve():
    LIMIT = 150000
    dpf = [0] * LIMIT

    for i in range(2, LIMIT):
        if dpf[i] == 0:  # i is prime
            for j in range(i, LIMIT, i):
                dpf[j] += 1

    consecutive = 0
    for i in range(2, LIMIT):
        if dpf[i] == 4:
            consecutive += 1
            if consecutive == 4:
                return i - 3
        else:
            consecutive = 0

answer = solve()
assert answer == 134043
print(answer)
