"""
Problem 233: Lattice Points on a Circle

Find sum of all n <= 10^11 such that the circle x^2+y^2=n^2 has exactly 420 lattice points.

r_2(n^2) = 4 * product(2*a_i + 1) for primes p_i ≡ 1 (mod 4) in factorization of n.
We need product(2*a_i + 1) = 105 = 3*5*7.

n = k * P where P = product of p_i^{a_i} with p_i ≡ 1 mod 4,
and k has no prime factor ≡ 1 mod 4.
"""

from itertools import permutations
import math

def sieve_primes(n):
    is_p = bytearray(b'\x01') * (n + 1)
    is_p[0] = is_p[1] = 0
    for i in range(2, int(n**0.5) + 1):
        if is_p[i]:
            is_p[i*i::i] = bytearray(len(is_p[i*i::i]))
    return [i for i in range(2, n + 1) if is_p[i]]

def solve():
    LIMIT = 10**11

    primes = sieve_primes(320000)
    primes_1mod4 = [p for p in primes if p % 4 == 1]

    def sum_no_1mod4(L):
        """Sum of k in [1, L] with no prime factor ≡ 1 mod 4."""
        if L <= 0:
            return 0
        bad = bytearray(L + 1)
        for p in primes_1mod4:
            if p > L:
                break
            for j in range(p, L + 1, p):
                bad[j] = 1
        return sum(i for i in range(1, L + 1) if not bad[i])

    def is_prime_check(n):
        if n < 2:
            return False
        for p in primes:
            if p * p > n:
                return True
            if n % p == 0:
                return False
        return True

    # Segmented sieve to find primes in [lo, hi]
    def seg_sieve(lo, hi):
        if lo > hi:
            return []
        sz = hi - lo + 1
        is_p = bytearray(b'\x01') * sz
        for p in primes:
            if p * p > hi:
                break
            start = ((lo + p - 1) // p) * p
            if start < lo:
                start += p
            for j in range(start, hi + 1, p):
                is_p[j - lo] = 0
        result = []
        for i in range(sz):
            if is_p[i] and (lo + i) > 1:
                result.append(lo + i)
        return result

    total = 0

    # Exponent patterns from factorizations of 105:
    # 105 -> (52), 3*35 -> (1,17), 5*21 -> (2,10), 7*15 -> (3,7), 3*5*7 -> (1,2,3)
    base_patterns = [
        (52,),
        (1, 17),
        (2, 10),
        (3, 7),
        (1, 2, 3),
    ]

    for base_pat in base_patterns:
        perms = set(permutations(base_pat))
        for perm in perms:
            def dfs(pos, prev_prime, current_P):
                nonlocal total
                if pos == len(perm):
                    L = LIMIT // current_P
                    s = sum_no_1mod4(L)
                    total += s * current_P
                    return

                e = perm[pos]
                is_last = (pos == len(perm) - 1)

                if is_last and e == 1:
                    # Last exponent is 1: iterate over primes ≡ 1 mod 4
                    max_p = LIMIT // current_P

                    # From small primes list
                    for p in primes_1mod4:
                        if p <= prev_prime:
                            continue
                        if p > max_p:
                            break
                        new_P = current_P * p
                        L = LIMIT // new_P
                        s = sum_no_1mod4(L)
                        total += s * new_P

                    # From segmented sieve for larger primes
                    sieve_lim = primes_1mod4[-1] if primes_1mod4 else 2
                    if max_p > sieve_lim:
                        seg_start = max(sieve_lim + 1, prev_prime + 1)
                        SEG_SIZE = 1000000
                        lo_seg = seg_start
                        while lo_seg <= max_p:
                            hi_seg = min(lo_seg + SEG_SIZE - 1, max_p)
                            seg_primes = seg_sieve(lo_seg, hi_seg)
                            for p in seg_primes:
                                if p <= prev_prime:
                                    continue
                                if p % 4 != 1:
                                    continue
                                new_P = current_P * p
                                L = LIMIT // new_P
                                s = sum_no_1mod4(L)
                                total += s * new_P
                            lo_seg = hi_seg + 1
                    return

                # General case
                for p in primes_1mod4:
                    if p <= prev_prime:
                        continue
                    pe = p ** e
                    if current_P * pe > LIMIT:
                        break
                    dfs(pos + 1, p, current_P * pe)

            dfs(0, 0, 1)

    print(total)

if __name__ == "__main__":
    solve()
