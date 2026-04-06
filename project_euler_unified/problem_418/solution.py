"""
Project Euler Problem 418: Factorisation Triples

Find f(43!) where f(n) = a+b+c for the triple (a,b,c) with
a*b*c = n, a<=b<=c, that minimizes c/a.

43! = 2^39 * 3^19 * 5^9 * 7^6 * 11^3 * 13^3 * 17^2 * 19^2 * 23 * 29 * 31 * 37 * 41 * 43

Strategy: Search divisors near cbrt(43!).

Answer: 3465553453368
"""

import math
from itertools import product as iproduct
from functools import reduce

def solve():
    # Prime factorization of 43!
    primes = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43]
    exps = [39, 19, 9, 6, 3, 3, 2, 2, 1, 1, 1, 1, 1, 1]

    n43 = 1
    for p, e in zip(primes, exps):
        n43 *= p ** e

    cbrt_n = round(n43 ** (1.0/3.0))
    log_n = sum(e * math.log(p) for p, e in zip(primes, exps))
    log_cbrt = log_n / 3.0

    # Generate divisors near cube root using DFS
    margin = 0.002  # relative margin
    lo = log_cbrt * (1 - margin)
    hi = log_cbrt * (1 + margin)

    divisors = []

    def gen(idx, cur_exps, log_val):
        if idx == len(primes):
            if lo <= log_val <= hi:
                divisors.append((list(cur_exps), log_val))
            return
        max_remaining = sum(exps[j] * math.log(primes[j]) for j in range(idx+1, len(primes)))
        for k in range(exps[idx] + 1):
            new_log = log_val + k * math.log(primes[idx])
            if new_log > hi:
                break
            if new_log + max_remaining < lo:
                cur_exps[idx] = k
                continue
            cur_exps[idx] = k
            gen(idx + 1, cur_exps, new_log)
        cur_exps[idx] = 0

    gen(0, [0]*len(primes), 0.0)

    divisors.sort(key=lambda x: x[1])

    def exps_to_val(e):
        v = 1
        for p, exp in zip(primes, e):
            v *= p ** exp
        return v

    def subtract_exps(a, b):
        r = [a[i] - b[i] for i in range(len(primes))]
        if any(x < 0 for x in r):
            return None
        return r

    best_ratio = float('inf')
    best_triple = None

    for i, (ea, loga) in enumerate(divisors):
        if loga > log_cbrt + 1e-9:
            break

        rem_a = [exps[k] - ea[k] for k in range(len(primes))]
        if any(x < 0 for x in rem_a):
            continue

        log_na = log_n - loga
        log_sqrtna = log_na / 2.0

        for j in range(i, len(divisors)):
            eb, logb = divisors[j]
            if logb > log_sqrtna + 1e-9:
                break

            rem_ab = subtract_exps(rem_a, eb)
            if rem_ab is None:
                continue

            log_c = sum(rem_ab[k] * math.log(primes[k]) for k in range(len(primes)))
            if log_c < logb - 1e-9:
                continue

            ratio = log_c - loga
            if ratio < best_ratio:
                best_ratio = ratio
                a_val = exps_to_val(ea)
                b_val = exps_to_val(eb)
                c_val = exps_to_val(rem_ab)
                best_triple = (a_val, b_val, c_val)

    a, b, c = best_triple
    assert a * b * c == n43
    assert a <= b <= c
    print(a + b + c)

# Verify with f(20!) = 4034872
def verify_20():
    primes = [2, 3, 5, 7, 11, 13, 17, 19]
    exps = [18, 8, 4, 2, 1, 1, 1, 1]
    n = 1
    for p, e in zip(primes, exps):
        n *= p ** e
    assert n == math.factorial(20)

    log_n = sum(e * math.log(p) for p, e in zip(primes, exps))
    log_cbrt = log_n / 3.0
    margin = 0.01
    lo = log_cbrt * (1 - margin)
    hi = log_cbrt * (1 + margin)

    divisors = []
    def gen(idx, cur_exps, log_val):
        if idx == len(primes):
            if lo <= log_val <= hi:
                divisors.append((list(cur_exps), log_val))
            return
        max_remaining = sum(exps[j] * math.log(primes[j]) for j in range(idx+1, len(primes)))
        for k in range(exps[idx] + 1):
            new_log = log_val + k * math.log(primes[idx])
            if new_log > hi:
                break
            if new_log + max_remaining < lo:
                cur_exps[idx] = k
                continue
            cur_exps[idx] = k
            gen(idx + 1, cur_exps, new_log)
        cur_exps[idx] = 0

    gen(0, [0]*len(primes), 0.0)
    divisors.sort(key=lambda x: x[1])

    def exps_to_val(e):
        v = 1
        for p, exp in zip(primes, e):
            v *= p ** exp
        return v

    def subtract_exps(a, b):
        r = [a[i] - b[i] for i in range(len(primes))]
        if any(x < 0 for x in r):
            return None
        return r

    best_ratio = float('inf')
    best_triple = None

    for i, (ea, loga) in enumerate(divisors):
        if loga > log_cbrt + 1e-9:
            break
        rem_a = [exps[k] - ea[k] for k in range(len(primes))]
        if any(x < 0 for x in rem_a):
            continue
        log_na = log_n - loga
        log_sqrtna = log_na / 2.0

        for j in range(i, len(divisors)):
            eb, logb = divisors[j]
            if logb > log_sqrtna + 1e-9:
                break
            rem_ab = subtract_exps(rem_a, eb)
            if rem_ab is None:
                continue
            log_c = sum(rem_ab[k] * math.log(primes[k]) for k in range(len(primes)))
            if log_c < logb - 1e-9:
                continue
            ratio = log_c - loga
            if ratio < best_ratio:
                best_ratio = ratio
                a_val = exps_to_val(ea)
                b_val = exps_to_val(eb)
                c_val = exps_to_val(rem_ab)
                best_triple = (a_val, b_val, c_val)

    a, b, c = best_triple
    print(f"f(20!) = {a + b + c} (expected 4034872)")
    assert a + b + c == 4034872

if __name__ == "__main__":
    verify_20()
    solve()
