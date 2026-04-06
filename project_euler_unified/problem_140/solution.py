"""
Problem 140: Modified Fibonacci Golden Nuggets

G(x) = x(1+3x)/(1-x-x^2). Find sum of first 30 golden nuggets.

Reduces to generalized Pell equation a^2 - 5*b^2 = 44 where a = 5n + 7.
"""

def solve():
    fund = [(7, 1), (8, 2), (13, 5), (17, 7)]
    nuggets = set()

    for a0, b0 in fund:
        for sa, sb in [(a0, b0), (-a0, -b0), (-a0, b0), (a0, -b0)]:
            a, b = sa, sb
            for _ in range(40):
                if a > 7 and (a - 7) % 5 == 0:
                    n = (a - 7) // 5
                    if n > 0:
                        nuggets.add(n)
                a, b = 9 * a + 20 * b, 4 * a + 9 * b

    sorted_nuggets = sorted(nuggets)[:30]
    return sum(sorted_nuggets)

if __name__ == "__main__":
    answer = solve()
assert answer == 5673835352990
print(answer)
