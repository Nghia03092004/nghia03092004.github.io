"""
Problem 134: Prime Pair Connection

For consecutive primes p1, p2 with 5 <= p1 <= 1000003, find S(p1,p2):
smallest positive integer ending in p1 and divisible by p2.
Sum all S values.
"""

def sieve(limit):
    is_prime = [True] * limit
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, limit, i):
                is_prime[j] = False
    return [i for i in range(2, limit) if is_prime[i]]

primes = sieve(1100000)

total = 0
s_values = []

for i in range(len(primes) - 1):
    p1 = primes[i]
    p2 = primes[i + 1]
    if p1 < 5:
        continue
    if p1 >= 1000003:
        break

    # Number of digits
    pow10 = 10 ** len(str(p1))

    # k = (-p1 * modinv(pow10, p2)) mod p2
    inv = pow(pow10, p2 - 2, p2)
    k = (-p1 * inv) % p2

    S = p1 + k * pow10
    total += S

    if i < 100:  # store first few for visualization
        s_values.append((p1, p2, S))

print(total)
