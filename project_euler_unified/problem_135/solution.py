"""
Problem 135: Same Differences

How many values of n < 10^6 have exactly 10 solutions to
x^2 - y^2 - z^2 = n where x, y, z are positive integers in arithmetic progression?

With x = y+d, z = y-d: n = y(4d - y), constraints: y/4 < d < y, n > 0.
"""


def solve():

    N = 1_000_000
    count = [0] * N

    for y in range(1, N):
        d_min = y // 4 + 1
        d_max = y - 1

        # n = y*(4d - y) < N => d < (N/y + y)/4
        upper = ((N - 1) // y + y) // 4
        if upper < d_max:
            d_max = upper

        for d in range(d_min, d_max + 1):
            n = y * (4 * d - y)
            if 0 < n < N:
                count[n] += 1

    result = sum(1 for i in range(1, N) if count[i] == 10)
    return result


answer = solve()
assert answer == 4989
print(answer)
