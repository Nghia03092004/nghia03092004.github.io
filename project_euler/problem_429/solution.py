"""
Project Euler Problem 429: Sum of Squares of Unitary Divisors

Find S(100_000_000!) mod 1_000_000_009 where S(n) is the sum of
squares of unitary divisors of n.

Key insight: If n = p1^a1 * p2^a2 * ... then
sigma*_2(n) = prod(1 + p_i^(2*a_i))

Use Legendre's formula for exponents in factorial,
and sieve for primes.
"""

def solve():
    N = 100_000_000
    MOD = 1_000_000_009

    # Sieve of Eratosthenes
    is_prime = bytearray(b'\x01') * (N + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(N**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    result = 1
    for p in range(2, N + 1):
        if not is_prime[p]:
            continue
        # Legendre's formula
        a = 0
        pk = p
        while pk <= N:
            a += N // pk
            pk *= p
        # Multiply (1 + p^(2a)) mod MOD
        result = result * (1 + pow(p, 2 * a, MOD)) % MOD

    print(result)

if __name__ == '__main__':
    solve()
