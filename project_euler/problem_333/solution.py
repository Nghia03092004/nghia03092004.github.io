"""
Problem 333: Special Partitions

Find the sum of all primes < 10^6 that can be written in exactly one way
as a sum of distinct terms of the form 2^a * 3^b.

Approach:
1. Generate all numbers 2^a * 3^b < 10^6.
2. 0-1 knapsack DP to count representations (capped at 2).
3. Sieve primes, sum those with exactly 1 representation.

Answer: 3053105
"""

def sieve_of_eratosthenes(limit):
    """Return a boolean array where is_prime[i] indicates if i is prime."""
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return is_prime

def solve():
    LIMIT = 1_000_000

    # Generate all 2^a * 3^b < LIMIT
    terms = []
    b3 = 1
    while b3 < LIMIT:
        val = b3
        while val < LIMIT:
            terms.append(val)
            val *= 2
        b3 *= 3
    terms.sort()
    print(f"Number of terms: {len(terms)}")

    # 0-1 knapsack: count representations (cap at 2)
    dp = bytearray(LIMIT)  # dp[s] = number of ways, capped at 2
    dp[0] = 1

    for t in terms:
        for s in range(LIMIT - 1 - t, -1, -1):
            if dp[s] > 0 and dp[s + t] < 2:
                dp[s + t] = min(2, dp[s + t] + dp[s])

    # Sieve for primes
    is_prime = sieve_of_eratosthenes(LIMIT)

    # Sum primes with exactly 1 representation
    answer = 0
    count = 0
    for p in range(2, LIMIT):
        if is_prime[p] and dp[p] == 1:
            answer += p
            count += 1

    print(f"Number of special primes: {count}")
    print(f"Answer: {answer}")
    return answer

if __name__ == "__main__":
    solve()
