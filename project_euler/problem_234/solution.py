"""
Problem 234: Semidivisible Numbers

Find the sum of all semidivisible numbers not exceeding 999966663333.
A number n is semidivisible if exactly one of lps(n), ups(n) divides n,
where lps(n) = largest prime <= sqrt(n), ups(n) = smallest prime >= sqrt(n).
"""

def sieve_primes(n):
    is_p = bytearray(b'\x01') * (n + 1)
    is_p[0] = is_p[1] = 0
    for i in range(2, int(n**0.5) + 1):
        if is_p[i]:
            is_p[i*i::i] = bytearray(len(is_p[i*i::i]))
    return [i for i in range(2, n + 1) if is_p[i]]

def sum_multiples(d, lo, hi):
    """Sum of multiples of d in open interval (lo, hi)."""
    first_k = lo // d + 1
    last_k = (hi - 1) // d
    if first_k > last_k:
        return 0
    cnt = last_k - first_k + 1
    return d * cnt * (first_k + last_k) // 2

def solve():
    LIMIT = 999966663333

    primes = sieve_primes(1100000)

    total = 0
    for i in range(len(primes) - 1):
        p = primes[i]
        q = primes[i + 1]
        lo = p * p
        hi = q * q

        if lo >= LIMIT:
            break
        if hi > LIMIT + 1:
            hi = LIMIT + 1

        sp = sum_multiples(p, lo, hi)
        sq = sum_multiples(q, lo, hi)
        spq = sum_multiples(p * q, lo, hi)
        total += sp + sq - 2 * spq

    print(total)

if __name__ == "__main__":
    solve()
