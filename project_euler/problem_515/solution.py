"""
Problem 515: Dissonant Numbers
Count integers with specific digit-sum properties across bases.
D(b, s, N) = count of 1 <= n <= N with digit_sum_base_b(n) = s.
"""

from functools import lru_cache

def digit_sum(n: int, b: int):
    """Compute the digit sum of n in base b."""
    s = 0
    while n > 0:
        s += n % b
        n //= b
    return s

def digits_in_base(n: int, b: int) -> list:
    """Return digits of n in base b, most significant first."""
    if n == 0:
        return [0]
    ds = []
    while n > 0:
        ds.append(n % b)
        n //= b
    return ds[::-1]

def D(b: int, s: int, N: int):
    """
    Count integers 1 <= n <= N with digit sum s in base b.
    Uses digit DP.
    """
    if N <= 0 or s < 0:
        return 0

    digs = digits_in_base(N, b)
    L = len(digs)

    # dp(pos, remaining_sum, tight) = count of valid completions
    from functools import lru_cache

    @lru_cache(maxsize=None)
    def dp(pos, rem, tight):
        if rem < 0:
            return 0
        if pos == L:
            return 1 if rem == 0 else 0
        limit = digs[pos] if tight else b - 1
        total = 0
        for d in range(0, limit + 1):
            total += dp(pos + 1, rem - d, tight and (d == limit))
        return total

    # Count for 0..N; subtract n=0 which has digit sum 0
    result = dp(0, s, True)
    if s == 0:
        result -= 1  # exclude n=0
    dp.cache_clear()
    return result

def D_brute(b: int, s: int, N: int):
    """Brute-force verification."""
    return sum(1 for n in range(1, N + 1) if digit_sum(n, b) == s)

# Verify
for b in [2, 10]:
    for s in range(0, 10):
        N = 100
        assert D(b, s, N) == D_brute(b, s, N), f"Mismatch b={b}, s={s}, N={N}"
print("Verification passed.")

# Compute some values
print("\nD(10, s, 10^6) for various s:")
for s in range(1, 55):
    val = D(10, s, 10**6)
    if val > 0:
        print(f"  D(10, {s}, 10^6) = {val}")

# The full problem asks for a specific sum; compute partial example
total = sum(D(10, s, 10**6) for s in range(1, 55))
print(f"\nSum over s: {total} (should be 10^6 = {10**6})")
