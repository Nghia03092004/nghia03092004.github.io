"""Problem 1: Multiples of 3 and 5"""

def sum_multiples(m: int, n: int) -> int:
    p = (n - 1) // m
    return m * p * (p + 1) // 2

def solve(n: int = 1000) -> int:
    return sum_multiples(3, n) + sum_multiples(5, n) - sum_multiples(15, n)

answer = solve()
assert answer == 233168
print(answer)
