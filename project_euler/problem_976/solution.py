"""
Problem 976: Perfect Power Detection

Count the number of distinct perfect powers a^b <= 10^18, where a >= 1 and b >= 2.

A perfect power is an integer that can be expressed as a^b for integers a >= 1, b >= 2.
Numbers like 64 = 2^6 = 4^3 = 8^2 should only be counted once.

Key results:
    - Enumerate all a^b for b = 2..60 (since 2^60 > 10^18), a = 2..iroot(N,b)
    - Use a set to deduplicate (e.g., 64 appears as 2^6, 4^3, 8^2)
    - Include 1 = 1^b for any b
    - answer = number of distinct perfect powers <= 10^18

Methods:
    1. iroot             — integer b-th root via Newton + binary search
    2. count_perfect_powers_brute — brute force enumeration with dedup set
    3. count_by_exponent — count of b-th powers for each exponent b
    4. perfect_power_density — fraction of integers <= N that are perfect powers
"""

from collections import Counter

def iroot(n, b):
    """Compute floor(n^(1/b)) exactly."""
    if n <= 0:
        return 0
    if b == 1:
        return n
    # Initial guess from floating point
    x = int(round(n ** (1.0 / b)))
    # Check neighborhood
    for dx in range(-3, 4):
        y = x + dx
        if y >= 1 and y ** b <= n and (y + 1) ** b > n:
            return y
    # Fallback: binary search
    lo, hi = 1, int(n ** (1.0 / b)) + 2
    while lo < hi:
        mid = (lo + hi + 1) // 2
        if mid ** b <= n:
            lo = mid
        else:
            hi = mid - 1
    return lo

def count_perfect_powers_brute(N, max_exp=60):
    """Enumerate all a^b <= N for a>=2, b>=2..max_exp. Return deduplicated set."""
    seen = set()
    for b in range(2, max_exp + 1):
        r = iroot(N, b)
        if r < 2:
            break
        for a in range(2, r + 1):
            v = a ** b
            if v <= N:
                seen.add(v)
    seen.add(1)  # 1^b for any b
    return seen

def count_by_exponent(N, max_exp=60):
    """For each b, how many distinct b-th powers a^b <= N (a >= 2)."""
    exponents = []
    counts = []
    for b in range(2, max_exp + 1):
        r = iroot(N, b)
        if r < 2:
            break
        exponents.append(b)
        counts.append(r - 1)  # a = 2..r
    return exponents, counts

def perfect_power_density(max_exp_scale=12):
    """Count perfect powers <= 10^k for k=1..max_exp_scale."""
    ks = []
    counts = []
    densities = []
    for k in range(1, max_exp_scale + 1):
        Nk = 10 ** k
        pp = count_perfect_powers_brute(Nk, max_exp=60)
        ks.append(k)
        counts.append(len(pp))
        densities.append(len(pp) / Nk)
    return ks, counts, densities

#  Verification
# Perfect powers <= 100: 1,4,8,9,16,25,27,32,36,49,64,81,100 = 13
pp_100 = count_perfect_powers_brute(100)
assert len(pp_100) == 13, f"Expected 13 perfect powers <= 100, got {len(pp_100)}"
assert 64 in pp_100  # 2^6 = 4^3 = 8^2
assert 27 in pp_100  # 3^3

# iroot checks
assert iroot(1000000, 2) == 1000
assert iroot(1000000, 3) == 100
assert iroot(1024, 10) == 2

#  Main computation
N = 10 ** 18
seen = count_perfect_powers_brute(N)
answer = len(seen)
print(answer)

#  Visualization — 4-panel figure
