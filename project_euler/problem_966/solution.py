"""
Problem 966: Abundant Number Chains

s(n) = sigma(n) - n (sum of proper divisors).
A number n is abundant if s(n) > n.
An "abundant chain" of length L starting at n means:
    n, s(n), s(s(n)), ..., each strictly increasing for L steps.

Count n <= 10^5 that start an abundant chain of length >= 5.

Key results:
    - s(n) computed via divisor sieve
    - Chain requires s(n) > n at every step for 5 consecutive iterations
    - Most chains terminate quickly; long chains are relatively rare

Methods:
    1. sieve_sum_of_proper_divisors — sieve s(n) up to limit
    2. chain_length                — compute chain length from starting n
    3. count_chains                — count all n <= N with chain length >= target
    4. chain_length_distribution   — distribution of chain lengths
"""

def sieve_sum_of_proper_divisors(limit):
    """Compute s(n) = sum of proper divisors for all n up to limit."""
    s = [0] * (limit + 1)
    for d in range(1, limit + 1):
        for multiple in range(2 * d, limit + 1, d):
            s[multiple] += d
    return s

def chain_length(n, s, max_val):
    """Return length of strictly increasing chain starting at n."""
    cur = n
    length = 0
    while cur <= max_val and cur > 0:
        nxt = s[cur]
        if nxt <= cur:
            break
        length += 1
        cur = nxt
    return length

def count_chains(N, target_len, sieve_limit):
    """Count n in [1, N] with chain length >= target_len."""
    s = sieve_sum_of_proper_divisors(sieve_limit)
    count = 0
    examples = []
    lengths = []
    for n in range(1, N + 1):
        cl = chain_length(n, s, sieve_limit)
        lengths.append(cl)
        if cl >= target_len:
            count += 1
            if len(examples) < 10:
                examples.append(n)
    return count, examples, lengths, s

def get_chain(n, s, max_val, max_steps=20):
    """Return the full chain values."""
    chain = [n]
    cur = n
    for _ in range(max_steps):
        if cur > max_val or cur <= 0:
            break
        nxt = s[cur]
        if nxt <= cur:
            break
        chain.append(nxt)
        cur = nxt
    return chain

# Verification with small known facts
s_small = sieve_sum_of_proper_divisors(100)
# s(12) = 1+2+3+4+6 = 16 > 12, so 12 is abundant
assert s_small[12] == 16
# s(6) = 1+2+3 = 6, perfect number
assert s_small[6] == 6
# s(28) = 1+2+4+7+14 = 28, perfect number
assert s_small[28] == 28
# s(1) = 0
assert s_small[1] == 0

# Compute answer
N = 10 ** 5
SIEVE_LIMIT = 10 ** 7
answer, examples, lengths, s = count_chains(N, 5, SIEVE_LIMIT)
print(answer)
