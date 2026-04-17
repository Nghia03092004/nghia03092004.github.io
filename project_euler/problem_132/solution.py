"""
Project Euler Problem 132: Large Repunit Factors

Find the sum of the first 40 prime factors of R(10^9).
A prime p (not 2, 3, or 5) divides R(K) iff 10^K = 1 (mod p).
"""

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit, i):
                is_prime[j] = False
    return [i for i in range(2, limit) if is_prime[i]]

K = 10**9
primes = sieve(200000)
factors = []

for p in primes:
    if p in (2, 3, 5):
        continue
    if pow(10, K, p) == 1:
        factors.append(p)
        if len(factors) == 40:
            break

print(sum(factors))
