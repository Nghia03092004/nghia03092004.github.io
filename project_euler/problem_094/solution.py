"""
Project Euler Problem 94: Almost Equilateral Triangles

Find the sum of perimeters of all almost equilateral triangles (sides a,a,a+/-1)
with integral area and perimeter not exceeding 10^9.

Both cases reduce to the Pell equation X^2 - 3Y^2 = 1 with fundamental
solution (2,1) and recurrence X' = 2X + 3Y, Y' = X + 2Y.

Answer: 518408346
"""


def solve(limit=1_000_000_000):
    total = 0
    x, y = 2, 1  # fundamental Pell solution

    while True:
        if x % 3 == 1:
            # Case b = a+1: a = (2x+1)/3, perimeter = 3a+1
            a = (2 * x + 1) // 3
            perim = 3 * a + 1
            if perim > limit:
                break
            if a >= 2:
                total += perim
        else:
            # Case b = a-1: a = (2x-1)/3, perimeter = 3a-1
            a = (2 * x - 1) // 3
            perim = 3 * a - 1
            if perim > limit:
                break
            if a >= 2:
                total += perim

        x, y = 2 * x + 3 * y, x + 2 * y

    return total


if __name__ == "__main__":
    answer = solve()
    assert answer == 518408346
    print(answer)
