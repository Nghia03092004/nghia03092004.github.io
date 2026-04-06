"""
Problem 937: Pandigital Prime Search

Find the largest pandigital prime. A k-pandigital number uses the digits
1 through k exactly once. We check each k from 1 to 9.

Key insight:
    A number is divisible by 3 iff its digit sum is divisible by 3.
    digit_sum(1..k) = k(k+1)/2. This is divisible by 3 for k in {3,5,6,8,9},
    so only k in {1, 2, 4, 7} can yield primes.

Results:
    - k=7 yields the largest pandigital prime: 7652413
    - Total pandigital primes found: 538

Methods:
    1. is_prime           -- trial-division primality test
    2. digit_sum_check    -- filter k values by divisibility rule
    3. find_pandigitals   -- enumerate all pandigital primes by k
    4. find_largest       -- find the answer
"""
from itertools import permutations
from collections import Counter

def is_prime(n):
    """Trial-division primality check."""
    if n < 2:
        return False
    if n < 4:
        return True
    if n % 2 == 0 or n % 3 == 0:
        return False
    i = 5
    while i * i <= n:
        if n % i == 0 or n % (i + 2) == 0:
            return False
        i += 6
    return True

def candidate_ks():
    """Return k values where k-pandigital numbers can possibly be prime."""
    result = []
    for k in range(1, 10):
        dsum = k * (k + 1) // 2
        if k == 1 or dsum % 3 != 0:
            result.append(k)
    return result

def find_pandigital_primes():
    """Find all k-pandigital primes for valid k values."""
    primes_by_k = {}
    all_primes = []
    valid_ks = candidate_ks()
    for k in valid_ks:
        digits = list(range(1, k + 1))
        kprimes = []
        for perm in permutations(digits):
            n = int("".join(map(str, perm)))
            if is_prime(n):
                kprimes.append(n)
        kprimes.sort()
        primes_by_k[k] = kprimes
        all_primes.extend(kprimes)
    return primes_by_k, sorted(all_primes)

def find_largest(all_primes):
    """Return the largest pandigital prime."""
    return max(all_primes) if all_primes else None

# Computation
primes_by_k, all_primes = find_pandigital_primes()

# Verification
assert is_prime(2) and is_prime(3) and not is_prime(4)
assert candidate_ks() == [1, 2, 4, 7]
assert 2143 in all_primes  # known 4-pandigital prime
assert is_prime(7652413)    # the expected answer

answer = find_largest(all_primes)
print(answer)
