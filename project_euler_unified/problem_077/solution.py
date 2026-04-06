"""
Problem 77: Prime Summations

What is the first value which can be written as the sum of primes
in over five thousand different ways?
Answer: 71
"""


def sieve(n):
    """Return list of primes up to n via the Sieve of Eratosthenes."""
    is_prime = [True] * (n + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(n**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, n + 1, i):
                is_prime[j] = False
    return [i for i in range(2, n + 1) if is_prime[i]]


def main():
    N = 100
    primes = sieve(N)

    # DP: unbounded knapsack with prime denominations
    dp = [0] * (N + 1)
    dp[0] = 1
    for p in primes:
        for j in range(p, N + 1):
            dp[j] += dp[j - p]

    # Find first n with q(n) > 5000
    for n in range(2, N + 1):
        if dp[n] > 5000:
            print(n)
            return

    print(-1)


if __name__ == "__main__":
    main()
