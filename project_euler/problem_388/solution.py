"""
Problem 388: Distinct Lines through Lattice Points

Consider all lattice points (a, b, c) with 0 <= a, b, c <= N.
From the origin O(0,0,0) a straight line is drawn to every point.
Let D(N) be the number of distinct such lines.

Given: D(1,000,000) = 831909254469114121
Find: D(10^10), answer as first 9 digits followed by last 9 digits.

Key identity (Mobius inversion):
    D(N) = sum_{d=1}^{N} mu(d) * ((floor(N/d) + 1)^3 - 1)

For large N, use sub-linear Mertens function computation.
"""

from math import gcd

def solve_brute_force(n: int):
    """Brute-force D(N) by enumerating primitive directions. O(N^3)."""
    count = 0
    for a in range(n + 1):
        for b in range(n + 1):
            for c in range(n + 1):
                if a == 0 and b == 0 and c == 0:
                    continue
                if gcd(gcd(a, b), c) == 1:
                    count += 1
    return count

def solve_sieve(n: int):
    """Compute D(N) via Mobius sieve. O(N) time, works for N up to ~10^7."""
    mu = [0] * (n + 1)
    mu[1] = 1
    is_prime = bytearray(n + 1)
    primes = []
    for i in range(2, n + 1):
        if not is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > n:
                break
            is_prime[i * p] = 1
            if i % p == 0:
                mu[i * p] = 0
                break
            mu[i * p] = -mu[i]

    result = 0
    for d in range(1, n + 1):
        if mu[d] == 0:
            continue
        q = n // d + 1
        result += mu[d] * (q * q * q - 1)
    return result

def solve_sublinear(n: int):
    """
    Compute D(N) in O(N^{2/3}) using sub-linear Mertens function.

    D(N) = sum_{d=1}^{N} mu(d) * ((floor(N/d)+1)^3 - 1)

    We group by q = floor(N/d) and use the Mertens function M(x) = sum_{d<=x} mu(d).
    """
    # Sieve threshold: N^{2/3}
    threshold = int(n ** (2 / 3)) + 1

    # Sieve mu and prefix sums up to threshold
    mu = [0] * (threshold + 1)
    mu[1] = 1
    is_prime = bytearray(threshold + 1)
    primes = []
    for i in range(2, threshold + 1):
        if not is_prime[i]:
            primes.append(i)
            mu[i] = -1
        for p in primes:
            if i * p > threshold:
                break
            is_prime[i * p] = 1
            if i % p == 0:
                mu[i * p] = 0
                break
            mu[i * p] = -mu[i]

    m_small = [0] * (threshold + 1)
    for i in range(1, threshold + 1):
        m_small[i] = m_small[i - 1] + mu[i]

    # Memoized Mertens for large values
    m_cache = {}

    def mertens(x):
        if x <= threshold:
            return m_small[x]
        if x in m_cache:
            return m_cache[x]
        result = 1
        k = 2
        while k <= x:
            q = x // k
            k_next = x // q + 1
            result -= (k_next - k) * mertens(q)
            k = k_next
        m_cache[x] = result
        return result

    # Main summation: group by q = floor(N/d)
    total = 0
    d = 1
    while d <= n:
        q = n // d
        d_max = n // q
        mu_sum = mertens(d_max) - mertens(d - 1)
        g = (q + 1) ** 3 - 1
        total += mu_sum * g
        d = d_max + 1

    return total

def format_answer(value: int) -> str:
    """Format as first 9 digits + last 9 digits."""
    s = str(value)
    return s[:9] + s[-9:]

# ---- Compute and verify ----

# Verify small cases against brute force
for test_n in [1, 2, 3, 5]:
    bf = solve_brute_force(test_n)
    sv = solve_sieve(test_n)
    assert bf == sv, f"Mismatch at N={test_n}: brute={bf}, sieve={sv}"

# Verify D(10^6) matches the given value
d_1m = solve_sieve(1_000_000)
assert d_1m == 831909254469114121, f"D(10^6) = {d_1m}, expected 831909254469114121"

# Compute D(10^10)
d_10b = solve_sublinear(10**10)
answer = format_answer(d_10b)
assert answer == "831907372805129931", f"Got {answer}, expected 831907372805129931"

print(answer)

# ---- Visualization ----
