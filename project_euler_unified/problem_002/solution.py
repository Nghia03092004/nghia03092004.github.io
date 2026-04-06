"""Problem 2: Even Fibonacci Numbers"""

def solve(limit: int = 4_000_000) -> int:
    a, b, total = 2, 8, 0
    while a <= limit:
        total += a
        a, b = b, 4 * b + a
    return total

answer = solve()
assert answer == 4613732
print(answer)
