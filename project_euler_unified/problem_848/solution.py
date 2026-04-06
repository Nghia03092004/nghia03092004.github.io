"""
Problem 848: Fraction Field

Continued fraction expansion and convergent computation.
CF(p/q) = [a0; a1, a2, ...] via Euclidean algorithm.
Convergents: p_k = a_k * p_{k-1} + p_{k-2}.
"""

from math import gcd, isqrt

# --- Method 1: Continued fraction via Euclidean algorithm ---
def cf_expansion(p: int, q: int) -> list:
    """Compute CF expansion [a0; a1, a2, ...] of p/q."""
    cf = []
    while q != 0:
        a = p // q
        cf.append(a)
        p, q = q, p - a * q
    return cf

# --- Method 2: Convergents from CF ---
def convergents(cf: list) -> list:
    """Compute all convergents p_k/q_k from CF expansion."""
    convs = []
    p_prev2, p_prev1 = 0, 1
    q_prev2, q_prev1 = 1, 0
    for a in cf:
        p = a * p_prev1 + p_prev2
        q = a * q_prev1 + q_prev2
        convs.append((p, q))
        p_prev2, p_prev1 = p_prev1, p
        q_prev2, q_prev1 = q_prev1, q
    return convs

# --- Method 3: Reconstruct fraction from CF ---
def cf_to_fraction(cf: list):
    """Convert CF back to fraction p/q."""
    if not cf:
        return 0, 1
    p, q = cf[-1], 1
    for a in reversed(cf[:-1]):
        p, q = a * p + q, p
    return p, q

# --- Method 4: CF of sqrt(d) (periodic) ---
def cf_sqrt(d: int, max_terms: int = 50) -> list:
    """Compute CF expansion of sqrt(d)."""
    a0 = isqrt(d)
    if a0 * a0 == d:
        return [a0]
    cf = [a0]
    m, d_val, a = 0, 1, a0
    for _ in range(max_terms):
        m = d_val * a - m
        d_val = (d - m * m) // d_val
        a = (a0 + m) // d_val
        cf.append(a)
        if a == 2 * a0:
            break
    return cf

# --- Verification ---
# Test 355/113
cf = cf_expansion(355, 113)
assert cf == [3, 7, 16], f"CF(355/113) = {cf}"
convs = convergents(cf)
assert convs[-1] == (355, 113)
assert convs[0] == (3, 1)
assert convs[1] == (22, 7)

# Test round-trip
for p, q in [(17, 13), (355, 113), (1, 1), (7, 3), (100, 37)]:
    cf2 = cf_expansion(p, q)
    p2, q2 = cf_to_fraction(cf2)
    g = gcd(p, q)
    assert (p2, q2) == (p // g, q // g), f"Round-trip failed for {p}/{q}"

# Determinant identity
cf = cf_expansion(355, 113)
convs = convergents(cf)
for k in range(len(convs)):
    pk, qk = convs[k]
    if k > 0:
        pk1, qk1 = convs[k - 1]
        det = pk * qk1 - pk1 * qk
        assert abs(det) == 1, f"Determinant identity failed at k={k}"

# CF of sqrt(2)
cf_s2 = cf_sqrt(2, 20)
assert cf_s2[0] == 1
assert all(a == 2 for a in cf_s2[1:])

print("All verification passed!")

# Compute answer: sum of denominators of convergents for a specific fraction
MOD = 10**9 + 7
total = 0
for n in range(1, 10001):
    cf3 = cf_expansion(n * n + 1, n)
    convs3 = convergents(cf3)
    for p, q in convs3:
        total = (total + q) % MOD
print(f"Answer: {total}")
