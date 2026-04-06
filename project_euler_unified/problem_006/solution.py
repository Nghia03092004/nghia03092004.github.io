"""Project Euler Problem 6: Sum Square Difference"""

def solve(n: int = 100) -> int:
    """D(n) = n(n-1)(n+1)(3n+2) / 12"""
    return n * (n - 1) * (n + 1) * (3 * n + 2) // 12

answer = solve()
assert answer == 25164150
print(answer)
