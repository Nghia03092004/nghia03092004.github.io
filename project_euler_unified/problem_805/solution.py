"""
Problem 805: Shifted Multiples
For a positive integer $n$ with leading digit $d_1$ and remaining digits forming number $m$, define $\text{shift}(n) = 10m + d_1$ (cyclic left shift of digits). Let $r(n) = n \bmod \text{shift}(n)$. Find $S(N) = \sum_{n=1}^{N} r(n)$.
"""

def solve(N=10000):
    """Sum of n mod shift(n) for n = 1 to N."""
    total = 0
    for n in range(1, N + 1):
        s = str(n)
        if len(s) == 1:
            continue  # shift(n) = n, remainder = 0
        shifted = int(s[1:] + s[0])
        if shifted == 0:
            continue
        total += n % shifted
    return total

answer = solve()
print(answer)
