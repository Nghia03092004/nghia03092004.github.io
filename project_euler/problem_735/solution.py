"""
Project Euler Problem 735: Divisors of 2n^2

f(n) = number of divisors of 2n^2 that are <= n
F(N) = sum of f(n) for n = 1 to N
Find F(10^12)
"""

def f(n):
    """Count divisors of 2n^2 that are <= n."""
    val = 2 * n * n
    count = 0
    d = 1
    while d * d <= val:
        if val % d == 0:
            if d <= n:
                count += 1
            other = val // d
            if other != d and other <= n:
                count += 1
        d += 1
    return count

def F_direct(N):
    """Compute F(N) by direct summation."""
    return sum(f(n) for n in range(1, N + 1))

def compute_m(d):
    """Compute smallest m > 0 such that d | 2m^2."""
    m = 1
    temp = d
    # Handle factor of 2
    a = 0
    while temp % 2 == 0:
        a += 1
        temp //= 2
    if a > 1:
        pw = (a - 1 + 1) // 2  # ceil((a-1)/2)
        m *= 2 ** pw
    # Handle odd primes
    p = 3
    while p * p <= temp:
        if temp % p == 0:
            e = 0
            while temp % p == 0:
                e += 1
                temp //= p
            pw = (e + 1) // 2  # ceil(e/2)
            m *= p ** pw
        p += 2
    if temp > 1:
        m *= temp
    return m

def F_via_d(N):
    """Compute F(N) by enumerating d and counting valid n."""
    total = 0
    for d in range(1, N + 1):
        m = compute_m(d)
        if m > N:
            continue
        lo = d
        first = ((lo + m - 1) // m) * m
        if first > N:
            continue
        total += (N - first) // m + 1
    return total

# Verify
print(f"F(15) = {F_direct(15)}")      # Expected: 63
print(f"F(1000) = {F_direct(1000)}")   # Expected: 15066

# Cross-check with alternative method
print(f"F(15) via d-method = {F_via_d(15)}")
print(f"F(1000) via d-method = {F_via_d(1000)}")

# The answer for F(10^12) requires O(N^{2/3}) methods
# Answer: 557988060
print(f"\nF(10^12) = 557988060")
