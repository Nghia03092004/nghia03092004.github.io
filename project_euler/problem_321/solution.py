"""
Problem 321: Swapping Counters

We seek positive integers n such that M(n) = n(n+2) is a triangular number.
The substitution x = 2k+1, y = n+1 transforms n(n+2) = k(k+1)/2 into the
generalised Pell equation x^2 - 8y^2 = -7, whose solutions fall into two
families, each satisfying n_{i+2} = 6 n_{i+1} - n_i + 4.
"""


def solve():
    # Family A seeds: n = 0, 3 (from fundamental solution (1,1))
    # Family B seeds: n = 1, 10 (from fundamental solution (5,2))
    a_prev, a_cur = 0, 3
    b_prev, b_cur = 1, 10

    vals = [a_cur, b_prev, b_cur]  # skip a_prev = 0
    for _ in range(23):
        a_prev, a_cur = a_cur, 6 * a_cur - a_prev + 4
        b_prev, b_cur = b_cur, 6 * b_cur - b_prev + 4
        vals.append(a_cur)
        vals.append(b_cur)

    vals.sort()
    print(sum(vals[:40]))


if __name__ == "__main__":
    solve()
