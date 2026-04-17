"""
Problem 485: Maximum number of divisors
d(i) = number of divisors of i.
M(n,k) = max(d(n), d(n+1), ..., d(n+k-1)).
Given: sum M(n,10) for n=1..100 = 432.
Find: sum M(n, 10^4) for n=1..10^8.
"""

from collections import deque

def divisor_count_sieve(limit: int) -> list:
    """Compute d(n) for n = 0..limit using a sieve."""
    d = [0] * (limit + 1)
    for j in range(1, limit + 1):
        for multiple in range(j, limit + 1, j):
            d[multiple] += 1
    return d

def sliding_window_max_sum(d: list, n_max: int, k: int):
    """Sum of sliding window maxima of d[1..n_max+k-1] with window size k."""
    dq = deque()  # stores indices, d-values decreasing
    total = 0

    for i in range(1, n_max + k):
        # Remove from back if current d[i] >= d[back]
        while dq and d[dq[-1]] <= d[i]:
            dq.pop()
        dq.append(i)

        # Remove from front if out of window
        # Window for position n: indices n..n+k-1
        # When i = n+k-1, window starts at n = i - k + 1
        n = i - k + 1
        if n >= 1:
            while dq[0] < n:
                dq.popleft()
            total += d[dq[0]]

    return total

def solve(n_max: int, k: int):
    """Solve the problem for given N and k."""
    limit = n_max + k - 1
    d = divisor_count_sieve(limit)
    return sliding_window_max_sum(d, n_max, k)

# Verify with small example
answer_check = solve(100, 10)
print(f"Sum M(n,10) for n=1..100 = {answer_check}")

# For the full problem: solve(10**8, 10**4) -- requires ~10^8 memory
# Uncomment for full computation (takes significant time/memory):
# answer = solve(10**8, 10**4)
# print(f"Sum M(n,10^4) for n=1..10^8 = {answer}")
