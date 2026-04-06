"""
Problem 200: Find the 200th Prime-proof Sqube Containing "200"

A sqube is p^2*q^3 or p^3*q^2 for distinct primes p, q.
Prime-proof means no single digit change produces a prime.
"""

def sieve_primes(limit):
    """Simple sieve of Eratosthenes."""
    is_prime = bytearray(b'\x01') * (limit + 1)
    is_prime[0] = is_prime[1] = 0
    for i in range(2, int(limit**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, limit + 1, i):
                is_prime[j] = 0
    return [i for i in range(2, limit + 1) if is_prime[i]]

def is_prime_miller_rabin(n):
    """Deterministic Miller-Rabin for n < 3.3e24."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    # Small primes check
    for p in [5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        if n == p:
            return True
        if n % p == 0:
            return False

    d = n - 1
    r = 0
    while d % 2 == 0:
        d //= 2
        r += 1

    for a in [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37]:
        if a >= n:
            continue
        x = pow(a, d, n)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True

def contains_200(n):
    return "200" in str(n)

def is_prime_proof(n):
    s = list(str(n))
    length = len(s)
    for i in range(length):
        orig = s[i]
        for d in '0123456789':
            if d == orig:
                continue
            if i == 0 and d == '0':
                continue
            s[i] = d
            val = int(''.join(s))
            if is_prime_miller_rabin(val):
                s[i] = orig
                return False
        s[i] = orig
    return True

def main():
    LIMIT = 10**12
    primes = sieve_primes(1000000)

    squbes = set()

    # p^2 * q^3
    for p in primes:
        p2 = p * p
        if p2 * 8 > LIMIT:
            break
        for q in primes:
            if p == q:
                continue
            q3 = q * q * q
            val = p2 * q3
            if val > LIMIT:
                break
            if contains_200(val):
                squbes.add(val)

    # p^3 * q^2
    for p in primes:
        p3 = p * p * p
        if p3 * 4 > LIMIT:
            break
        for q in primes:
            if p == q:
                continue
            q2 = q * q
            val = p3 * q2
            if val > LIMIT:
                break
            if contains_200(val):
                squbes.add(val)

    candidates = sorted(squbes)

    count = 0
    for val in candidates:
        if is_prime_proof(val):
            count += 1
            if count == 200:
                print(val)
                return

    print("Not found within limit")

if __name__ == "__main__":
    main()
