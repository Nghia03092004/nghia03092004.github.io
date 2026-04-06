"""
Problem 511: Sequences with Nice Divisibility Properties
Count n-tuples (a_1,...,a_n) with 1 <= a_i <= m and sum divisible by k.
Uses the roots-of-unity filter (discrete Fourier transform technique).
"""

import numpy as np
from math import gcd

def S_simple(n: int, k: int):
    """
    Count n-tuples from {1,...,k} with sum divisible by k.
    Answer: k^(n-1) (since choosing n-1 values freely determines the last).
    """
    return pow(k, n - 1)

def S_general_float(n: int, k: int, m: int):
    """
    Count n-tuples from {1,...,m} with sum divisible by k.
    Uses roots-of-unity filter with floating-point arithmetic.
    For exact results, use modular arithmetic version.
    """
    omega = np.exp(2j * np.pi / k)
    total = 0.0

    for j in range(k):
        if j == 0:
            gj = m
        else:
            w = omega ** j
            # g_j = sum_{a=1}^{m} w^a = w * (1 - w^m) / (1 - w)
            gj = w * (1 - w ** m) / (1 - w)
        total += gj ** n

    result = total / k
    return int(round(result.real))

def S_general_mod(n: int, k: int, m: int, p: int):
    """
    Count n-tuples from {1,...,m} with sum divisible by k, modulo prime p.
    Requires k | (p - 1) so that a k-th root of unity exists mod p.
    """
    # Find a primitive k-th root of unity mod p
    # First find a primitive root mod p, then take its ((p-1)/k)-th power
    assert (p - 1) % k == 0, f"k={k} does not divide p-1={p-1}"

    # Find primitive root of p
    def primitive_root(p):
        if p == 2:
            return 1
        phi = p - 1
        # Factor phi
        factors = set()
        n = phi
        for d in range(2, int(n**0.5) + 1):
            while n % d == 0:
                factors.add(d)
                n //= d
            if n == 1:
                break
        if n > 1:
            factors.add(n)

        for g in range(2, p):
            ok = True
            for f in factors:
                if pow(g, phi // f, p) == 1:
                    ok = False
                    break
            if ok:
                return g
        return -1

    g = primitive_root(p)
    omega = pow(g, (p - 1) // k, p)

    total = 0
    inv_k = pow(k, p - 2, p)  # modular inverse of k

    for j in range(k):
        if j == 0:
            gj = m % p
        else:
            w = pow(omega, j, p)
            wm = pow(w, m, p)
            # gj = w * (1 - w^m) / (1 - w) mod p
            num = w * (1 - wm) % p
            den = (1 - w) % p
            gj = num * pow(den, p - 2, p) % p

        total = (total + pow(gj, n, p)) % p

    return total * inv_k % p

def brute_force(n: int, k: int, m: int):
    """Brute-force for small n, m: enumerate all tuples."""
    if n == 0:
        return 1 if 0 % k == 0 else 0

    count = 0

    def recurse(depth, running_sum):
        nonlocal count
        if depth == n:
            if running_sum % k == 0:
                count += 1
            return
        for a in range(1, m + 1):
            recurse(depth + 1, running_sum + a)

    recurse(0, 0)
    return count

# Verify simple case
print("Simple case S(n, k) = k^(n-1):")
for n in range(1, 6):
    for k in [2, 3, 5]:
        exact = brute_force(n, k, k)
        formula = S_simple(n, k)
        status = "OK" if exact == formula else "MISMATCH"
        print(f"  S({n}, {k}) = {formula} (brute={exact}) [{status}]")

# Verify general case
print("\nGeneral case S(n, k, m):")
for n, k, m in [(2, 3, 5), (3, 4, 6), (2, 5, 7), (3, 3, 4)]:
    exact = brute_force(n, k, m)
    approx = S_general_float(n, k, m)
    status = "OK" if exact == approx else f"MISMATCH(got {approx})"
    print(f"  S({n}, {k}, {m}) = {exact} (float={approx}) [{status}]")

# Modular computation for larger values
# Need p such that k | (p-1)
# For k=1234567, we need p with (p-1) % 1234567 == 0
# Example with moderate k
k_test = 7
n_test = 100
m_test = 20
p_mod = 10**9 + 7  # (10^9+6) % 7 = (10^9+6) mod 7

# Check if 7 | (p-1)
if (p_mod - 1) % k_test == 0:
    result_mod = S_general_mod(n_test, k_test, m_test, p_mod)
    result_float = S_general_float(n_test, k_test, m_test)
    print(f"\nS({n_test}, {k_test}, {m_test}) mod {p_mod} = {result_mod}")
else:
    print(f"\nCannot use p={p_mod} for k={k_test} (k does not divide p-1)")
