"""
Problem 520: Simbers
A simber is a number where every digit appears an even number of times.
Count simbers up to 10^n using digit DP with parity bitmask.
"""

from functools import lru_cache

def count_simbers_up_to(N: int):
    """
    Count simbers in [1, N] using digit DP.
    Parity bitmask: bit i = 1 means digit i has appeared odd number of times.
    A simber has mask = 0 at the end.
    """
    digits = [int(d) for d in str(N)]
    L = len(digits)

    @lru_cache(maxsize=None)
    def dp(pos, mask, tight, started):
        if pos == L:
            return 1 if (started and mask == 0) else 0
        limit = digits[pos] if tight else 9
        total = 0
        for d in range(0, limit + 1):
            new_tight = tight and (d == limit)
            if not started and d == 0:
                total += dp(pos + 1, mask, new_tight, False)
            else:
                new_mask = mask ^ (1 << d)
                total += dp(pos + 1, new_mask, new_tight, True)
        return total

    result = dp(0, 0, True, False)
    dp.cache_clear()
    return result

def count_simbers_brute(N: int):
    """Brute-force for verification."""
    count = 0
    for n in range(1, N + 1):
        s = str(n)
        if all(s.count(d) % 2 == 0 for d in set(s)):
            count += 1
    return count

# Verify on small inputs
for N in [10, 100, 1000, 10000]:
    dp_ans = count_simbers_up_to(N)
    bf_ans = count_simbers_brute(N)
    assert dp_ans == bf_ans, f"Mismatch at N={N}: {dp_ans} vs {bf_ans}"
    print(f"Q(N={N}): {dp_ans}")

# Compute Q(10^n) for n = 1..15
print("\nQ(10^n) for various n:")
results = {}
for n in range(1, 16):
    N = 10**n
    q = count_simbers_up_to(N)
    results[n] = q
    print(f"  Q(10^{n}) = {q}")
