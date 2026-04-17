"""
Project Euler Problem 131: Prime Cube Partnership

A prime p satisfies n^3 + n^2*p = k^3 for some positive integer n if and only
if p = (s+1)^3 - s^3 = 3s^2 + 3s + 1 for some positive integer s.
Count such primes below 10^6.
"""

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return is_prime

LIMIT = 1_000_000
is_prime = sieve(LIMIT)

count = 0
s = 1
while True:
    p = 3 * s * s + 3 * s + 1
    if p >= LIMIT:
        break
    if is_prime[p]:
        count += 1
    s += 1

print(count)
