"""
Problem 381: (prime-k) factorial

For prime p, S(p) = sum of (p-k)! mod p for k=1..5.
Using Wilson's theorem to derive closed forms, then sieve primes.

Answer: 139602943319822
"""

def solve():
    """
    By Wilson's theorem, (p-1)! = -1 mod p.

    Working backwards:
    (p-1)! = p-1 mod p
    (p-2)! = 1 mod p
    (p-3)! = (p-1)/2 mod p  (i.e., -inv(2) mod p)
    (p-4)! = -(p-1)*inv(6) mod p
    (p-5)! = (p-1)*inv(24) mod p

    Summing: S(p) = -3 * inv(8) mod p

    We sieve primes up to 10^8 and compute S(p) for each.
    """
    N = 10**8
    # Sieve of Eratosthenes
    sieve = bytearray([1]) * N
    sieve[0] = sieve[1] = 0
    for i in range(2, int(N**0.5) + 1):
        if sieve[i]:
            sieve[i*i::i] = bytearray(len(sieve[i*i::i]))

    total = 0
    for p in range(5, N):
        if sieve[p]:
            inv2 = (p + 1) // 2
            inv8 = pow(inv2, 3, p)
            s = (-3 * inv8) % p
            total += s

    print(total)

if __name__ == "__main__":
    solve()
