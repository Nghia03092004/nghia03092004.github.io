import math

def sieve_of_eratosthenes(limit):
    is_prime = [True] * (limit + 1)
    is_prime[0] = is_prime[1] = False
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i * i, limit + 1, i):
                is_prime[j] = False
    return [i for i in range(2, limit + 1) if is_prime[i]]

n = 10001
upper = int(n * (math.log(n) + math.log(math.log(n)))) + 100
primes = sieve_of_eratosthenes(upper)
print(primes[n - 1])
