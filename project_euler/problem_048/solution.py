"""
Problem 48: Self Powers

Find the last ten digits of 1^1 + 2^2 + 3^3 + ... + 1000^1000.
Uses modular exponentiation with M = 10^10.
"""

def solve():
    M = 10**10
    total = 0
    for k in range(1, 1001):
        total = (total + pow(k, k, M)) % M
    return total

answer = solve()
assert answer == 9110846700
print(answer)
