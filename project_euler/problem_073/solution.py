"""
Project Euler Problem 73: Counting Fractions in a Range

Count reduced fractions n/d strictly between 1/3 and 1/2 with d <= 12000.

By Theorem 3 (Mobius Summation), we sieve mu and sum over divisor classes.
"""


def solve():
    N = 12000

    # Sieve the Mobius function
    mu = [1] * (N + 1)
    is_prime = [True] * (N + 1)

    for p in range(2, N + 1):
        if is_prime[p]:
            for m in range(2 * p, N + 1, p):
                is_prime[m] = False
            for m in range(p * p, N + 1, p * p):
                mu[m] = 0
            for m in range(p, N + 1, p):
                mu[m] *= -1

    count = 0
    for k in range(1, N + 1):
        if mu[k] == 0:
            continue
        limit = N // k
        c = 0
        for d in range(2, limit + 1):
            lo = d // 3 + 1       # smallest n > d/3
            hi = (d - 1) // 2     # largest n < d/2
            if hi >= lo:
                c += hi - lo + 1
        count += mu[k] * c

    return count


if __name__ == "__main__":
    answer = solve()
    assert answer == 7295372
    print(answer)
