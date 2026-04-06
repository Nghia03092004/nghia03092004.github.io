"""
Problem 807: Loops of Ropes
Consider $n$ ropes with $2n$ ends randomly paired. Let $E(n)$ be the expected number of loops. Find $\lfloor E(10^6) \rfloor$.
"""

def solve(n=10**6):
    """Expected number of loops from n ropes."""
    E = 0.0
    for k in range(1, n + 1):
        E += 1.0 / (2 * k - 1)
    return int(E)

answer = solve()
print(answer)
