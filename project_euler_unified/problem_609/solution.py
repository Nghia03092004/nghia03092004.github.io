"""
Problem 609: Pi Sequences
Build chains n -> pi(n) -> pi(pi(n)) -> ... where pi is prime counting function.
"""

def sieve_pi(N):
    """Compute pi(n) for all n up to N."""
    is_prime = [False] * (N + 1)
    for i in range(2, N + 1):
        is_prime[i] = True
    for i in range(2, int(N**0.5) + 1):
        if is_prime[i]:
            for j in range(i*i, N + 1, i):
                is_prime[j] = False
    pi = [0] * (N + 1)
    for i in range(1, N + 1):
        pi[i] = pi[i-1] + (1 if is_prime[i] else 0)
    return pi, is_prime

def chain_length(n, pi):
    """Length of chain n -> pi(n) -> pi(pi(n)) -> ... until reaching fixed point."""
    length = 0
    while n > 1:
        n = pi[n]
        length += 1
    return length

N = 10000
pi, is_prime = sieve_pi(N)

# Verify
assert pi[10] == 4  # primes <= 10: 2,3,5,7
assert pi[100] == 25

# Chain lengths
chains = [chain_length(n, pi) for n in range(2, N + 1)]
print(f"Max chain length for n <= {N}: {max(chains)}")
print(f"Average chain length: {sum(chains)/len(chains):.2f}")

# Count by chain length
from collections import Counter
counts = Counter(chains)
for length in sorted(counts):
    print(f"  Chain length {length}: {counts[length]} numbers")

# Product of counts mod 10^9+7
MOD = 10**9 + 7
product = 1
for length in sorted(counts):
    product = (product * counts[length]) % MOD
print(f"Product of counts mod 10^9+7: {product}")
