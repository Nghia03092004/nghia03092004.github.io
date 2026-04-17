"""
Problem 969: Additive Prime Counting

Find the number of primes p < 10^6 such that the digit sum of p is also prime.
These are called "additive primes."

Key results:
    - A prime p is "additive" if digit_sum(p) is also prime.
    - For p < 10^6, digit sums range from 2 to 54, so we only need
      small primes up to 54.
    - answer = count of additive primes below 10^6

Methods:
    1. sieve_of_eratosthenes  — standard boolean sieve
    2. digit_sum              — iterative digit extraction
    3. count_additive_primes  — main scan with digit-sum primality check
    4. additive_ratio_by_magnitude — fraction of primes that are additive per decade
    5. digit_sum_histogram    — full digit-sum frequency among primes
"""

from math import isqrt
from collections import Counter

def sieve_of_eratosthenes(n):
    """Return bytearray where s[i] is 1 iff i is prime, for 0 <= i <= n."""
    s = bytearray(b'\x01') * (n + 1)
    s[0] = s[1] = 0
    for i in range(2, isqrt(n) + 1):
        if s[i]:
            s[i * i :: i] = bytearray(len(s[i * i :: i]))
    return s

def digit_sum(n):
    """Sum of decimal digits of n."""
    s = 0
    while n:
        s += n % 10
        n //= 10
    return s

def count_additive_primes(N):
    """Return (count, is_prime_array, digit_sum_counter, additive_ds_counter)."""
    is_p = sieve_of_eratosthenes(N - 1)
    small_primes = set(p for p in range(2, 55) if is_p[p])
    count = 0
    ds_all = Counter()       # digit sums of ALL primes
    ds_additive = Counter()  # digit sums of additive primes only
    for p in range(2, N):
        if is_p[p]:
            d = digit_sum(p)
            ds_all[d] += 1
            if d in small_primes:
                count += 1
                ds_additive[d] += 1
    return count, is_p, ds_all, ds_additive

def additive_ratio_by_magnitude(is_p, max_exp=6):
    """Fraction of primes that are additive in [2, 10^k) for k=1..max_exp."""
    small_primes = set(p for p in range(2, 55) if is_p[p])
    exponents, ratios = [], []
    for k in range(1, max_exp + 1):
        bound = 10 ** k
        total = 0
        additive = 0
        for p in range(2, min(bound, len(is_p))):
            if is_p[p]:
                total += 1
                if digit_sum(p) in small_primes:
                    additive += 1
        exponents.append(k)
        ratios.append(additive / total if total else 0)
    return exponents, ratios

def digit_sum_histogram(ds_counter):
    """Sorted keys and values from a digit-sum counter."""
    keys = sorted(ds_counter.keys())
    vals = [ds_counter[k] for k in keys]
    return keys, vals

#  Verification
# Small known values: additive primes below 100
# 2,3,5,7,11,23,29,41,43,47,61,67,83,89 => 14
is_p_small = sieve_of_eratosthenes(99)
sp = set(p for p in range(2, 55) if is_p_small[p])
small_count = sum(1 for p in range(2, 100) if is_p_small[p] and digit_sum(p) in sp)
assert small_count == 14, f"Expected 14 additive primes < 100, got {small_count}"

#  Main computation
N = 10 ** 6
answer, is_p, ds_all, ds_additive = count_additive_primes(N)
print(answer)

#  Visualization — 4-panel figure
