"""
Problem 231: The Prime Factorisation of Binomial Coefficients
Find the sum of terms in the prime factorisation of C(20000000, 15000000).
"""

def sieve(n):
    """Sieve of Eratosthenes returning list of primes up to n."""
    is_prime = bytearray(b'\x01') * (n + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(n**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))
    return [i for i in range(2, n + 1) if is_prime[i]]

def exponent_in_factorial(n, p):
    """Compute the exponent of prime p in n! using Legendre's formula."""
    exp = 0
    pk = p
    while pk <= n:
        exp += n // pk
        pk *= p
    return exp

def solve():
    N = 20000000
    K = 15000000
    M = N - K  # 5000000

    primes = sieve(N)

    answer = 0
    for p in primes:
        e = exponent_in_factorial(N, p) - exponent_in_factorial(K, p) - exponent_in_factorial(M, p)
        if e > 0:
            answer += e * p

    print(answer)

if __name__ == "__main__":
    solve()
