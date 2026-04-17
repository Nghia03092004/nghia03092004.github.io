"""
Problem 963: Palindromic Primes in Bases

Count primes < 10^6 that are palindromes in at least 2 bases from {2,...,16}.

Algorithm: sieve + base conversion + palindrome check.
Complexity: O(N log log N + pi(N) * B * log N).
"""

from math import isqrt
from collections import Counter

def sieve(n):
    s = bytearray(b'\x01') * (n + 1)
    s[0] = s[1] = 0
    for i in range(2, isqrt(n) + 1):
        if s[i]:
            s[i*i::i] = bytearray(len(s[i*i::i]))
    return [i for i in range(2, n + 1) if s[i]]

def to_base(n, b):
    """Convert n to list of digits in base b (least significant first)."""
    if n == 0:
        return [0]
    digits = []
    while n > 0:
        digits.append(n % b)
        n //= b
    return digits

def is_palindrome_base(n, b):
    """Check if n is a palindrome in base b."""
    digits = to_base(n, b)
    return digits == digits[::-1]

def solve(N=10**6):
    primes = sieve(N - 1)
    count = 0
    base_counts = Counter()
    palindrome_counts = Counter()
    for p in primes:
        pal_bases = []
        for b in range(2, 17):
            if is_palindrome_base(p, b):
                pal_bases.append(b)
                base_counts[b] += 1
        if len(pal_bases) >= 2:
            count += 1
            palindrome_counts[len(pal_bases)] += 1
    return count, base_counts, palindrome_counts

# --- Compute answer ---
answer, base_counts, pal_counts = solve()
print(answer)
