"""
Problem 216: Investigating the Primality of Values of 2n^2 - 1

Count how many t(n) = 2n^2 - 1 are prime for 2 <= n <= 50,000,000.

Approach: Sieve - for each prime p (where 2 is a QR mod p), find roots of
2x^2 = 1 (mod p) and mark those n-values as having composite t(n).

Answer: 5437849
"""

import math

def solve():
    LIMIT = 50_000_000
    SIEVE_LIMIT = int(math.isqrt(2 * LIMIT * LIMIT)) + 1

    # Sieve of Eratosthenes for primes up to SIEVE_LIMIT
    is_prime = bytearray(b'\x01') * (SIEVE_LIMIT + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(math.isqrt(SIEVE_LIMIT)) + 1):
        if is_prime[i]:
            is_prime[i*i::i] = bytearray(len(is_prime[i*i::i]))

    # is_composite[n] = 1 means t(n) is composite
    is_composite = bytearray(LIMIT + 1)

    def modpow(base, exp, mod):
        result = 1
        base %= mod
        while exp > 0:
            if exp & 1:
                result = result * base % mod
            base = base * base % mod
            exp >>= 1
        return result

    def tonelli_shanks(a, p):
        """Find x such that x^2 = a mod p."""
        if a == 0:
            return 0
        if p == 2:
            return a % 2

        # Check QR
        if modpow(a, (p - 1) // 2, p) != 1:
            return -1

        if p % 4 == 3:
            return modpow(a, (p + 1) // 4, p)

        # Factor p-1 = 2^s * q
        s, q = 0, p - 1
        while q % 2 == 0:
            s += 1
            q //= 2

        # Find non-residue
        z = 2
        while modpow(z, (p - 1) // 2, p) != p - 1:
            z += 1

        M = s
        c = modpow(z, q, p)
        t = modpow(a, q, p)
        R = modpow(a, (q + 1) // 2, p)

        while True:
            if t == 1:
                return R
            i = 1
            tmp = t * t % p
            while tmp != 1:
                tmp = tmp * tmp % p
                i += 1
            b = c
            for _ in range(M - i - 1):
                b = b * b % p
            M = i
            c = b * b % p
            t = t * c % p
            R = R * b % p

    for p in range(3, SIEVE_LIMIT + 1):
        if not is_prime[p]:
            continue
        # 2 is QR mod p iff p = +/-1 mod 8
        if p % 8 not in (1, 7):
            continue

        # Find root of n^2 = (p+1)/2 mod p
        half_inv = (p + 1) // 2
        r = tonelli_shanks(half_inv, p)
        if r < 0:
            continue

        # Check if t(n) = p for some n (don't mark those)
        p_check = -1
        val = (p + 1) // 2
        sq = int(math.isqrt(val))
        if sq * sq == val and 2 <= sq <= LIMIT:
            p_check = sq

        for root in (r, p - r):
            if root == 0:
                continue
            # Mark all n = root, root+p, root+2p, ... as composite
            start = root if root >= 2 else root + p * ((2 - root + p - 1) // p)
            for n in range(start, LIMIT + 1, p):
                if n != p_check:
                    is_composite[n] = 1

    count = 0
    for n in range(2, LIMIT + 1):
        if not is_composite[n]:
            count += 1

    print(count)

if __name__ == "__main__":
    solve()
