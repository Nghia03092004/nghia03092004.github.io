"""
Problem 179: Consecutive Positive Divisors
Count n in (1, 10^7) where d(n) = d(n+1).
"""

def solve_sieve(N: int = 10_000_000):
    d = [0] * (N + 2)
    for k in range(1, N + 2):
        for j in range(k, N + 2, k):
            d[j] += 1
    count = sum(1 for n in range(2, N) if d[n] == d[n + 1])
    return count, d

def solve_brute(N: int = 1000):
    def divisor_count(n):
        c = 0
        for d in range(1, n + 1):
            if n % d == 0: c += 1
        return c
    return sum(1 for n in range(2, N) if divisor_count(n) == divisor_count(n + 1))

# Verify small case
assert solve_brute(100) == sum(1 for n in range(2, 100)
    if sum(1 for d in range(1, n+1) if n%d==0) == sum(1 for d in range(1, n+2) if (n+1)%d==0))

answer, d = solve_sieve()
assert answer == 986262, f"Expected 986262, got {answer}"
print(answer)
