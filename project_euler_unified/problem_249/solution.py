def solve():
    """
    Problem 249: Prime Subset Sums
    Count subsets of primes below 5000 whose sum is prime, mod 10^16.
    """
    MOD = 10**16
    LIMIT = 5000

    # Sieve to get all primes below 5000
    is_prime_sieve = [True] * LIMIT
    is_prime_sieve[0] = is_prime_sieve[1] = False
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime_sieve[i]:
            for j in range(i*i, LIMIT, i):
                is_prime_sieve[j] = False

    primes = [i for i in range(2, LIMIT) if is_prime_sieve[i]]
    S = sum(primes)

    # Sieve up to S
    is_prime = bytearray(S + 1)
    for i in range(2, S + 1):
        is_prime[i] = 1
    for i in range(2, int(S**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, S + 1, i):
                is_prime[j] = 0

    # DP (0/1 knapsack)
    dp = [0] * (S + 1)
    dp[0] = 1

    for p in primes:
        for s in range(S, p - 1, -1):
            dp[s] = (dp[s] + dp[s - p]) % MOD

    answer = 0
    for s in range(2, S + 1):
        if is_prime[s]:
            answer = (answer + dp[s]) % MOD

    print(answer)

solve()
