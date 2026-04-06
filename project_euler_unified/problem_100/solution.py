"""
Project Euler Problem 100: Arranged Probability

Find the number of blue discs b in the first arrangement with n > 10^12
total discs such that P(two blue) = b(b-1)/(n(n-1)) = 1/2.

The condition 2b(b-1) = n(n-1) reduces to the negative Pell equation
2x^2 - y^2 = 1 via x = 2b-1, y = 2n-1. The second-order recurrence
b_{k+1} = 6*b_k - b_{k-1} - 2 (and same for n) generates all solutions.

Answer: 756872327473
"""


def solve():
    threshold = 10**12

    b_prev, n_prev = 1, 1
    b_curr, n_curr = 3, 4

    while n_curr <= threshold:
        b_next = 6 * b_curr - b_prev - 2
        n_next = 6 * n_curr - n_prev - 2
        b_prev, n_prev = b_curr, n_curr
        b_curr, n_curr = b_next, n_next

    print(b_curr)


if __name__ == "__main__":
    solve()
