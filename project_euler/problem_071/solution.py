"""
Project Euler Problem 71: Ordered Fractions

Find the numerator of the fraction immediately to the left of 3/7
in the Farey sequence F_{1,000,000}.

By Theorem 2, d is the largest integer <= N with d = 5 (mod 7),
and n = (3d - 1) / 7.  Here we use a brute-force scan for verification.
"""


def solve():
    LIMIT = 1_000_000
    best_n, best_d = 0, 1

    for d in range(1, LIMIT + 1):
        if d % 7 == 0:
            continue
        n = (3 * d - 1) // 7
        # Compare n/d > best_n/best_d via cross-multiplication
        if n * best_d > best_n * d:
            best_n, best_d = n, d

    return best_n


if __name__ == "__main__":
    answer = solve()
    print(answer)
