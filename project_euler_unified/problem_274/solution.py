"""
Problem 274: Divisibility Multipliers

For each prime p coprime to 10, the divisibility multiplier m(p) is the
smallest positive integer m < p such that for the operation
f(n) = floor(n/10) + (n mod 10) * m, if p | n then p | f(n).

This is equivalent to m(p) = 10^{-1} mod p (modular inverse of 10 mod p).

Find the sum of m(p) for all primes p coprime to 10 with p < 10^7.
"""

def solve():
    LIMIT = 10**7

    # Sieve of Eratosthenes
    is_prime = bytearray(b'\x01') * LIMIT
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(LIMIT**0.5) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    total = 0
    for p in range(3, LIMIT):
        if p == 5:
            continue
        if is_prime[p]:
            total += pow(10, -1, p)

    print(total)

if __name__ == "__main__":
    solve()
