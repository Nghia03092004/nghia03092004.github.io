"""
Problem 50: Consecutive Prime Sum

Which prime below one million can be written as the sum of the most
consecutive primes?
"""

def solve():
    LIMIT = 1_000_000

    is_prime = [True] * LIMIT
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, LIMIT, i):
                is_prime[j] = False

    primes = [i for i in range(2, LIMIT) if is_prime[i]]
    n = len(primes)

    prefix = [0] * (n + 1)
    for i in range(n):
        prefix[i + 1] = prefix[i] + primes[i]

    best_len = 0
    best_prime = 0

    for length in range(n, best_len, -1):
        for start in range(n - length + 1):
            s = prefix[start + length] - prefix[start]
            if s >= LIMIT:
                break
            if is_prime[s]:
                best_len = length
                best_prime = s
                break
        if best_len == length:
            break

    return best_prime

answer = solve()
assert answer == 997651
print(answer)
