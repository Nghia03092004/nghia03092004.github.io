#!/usr/bin/env python3
"""
Project Euler Problem 404: Crisscross Ellipses

E_a: x^2 + 4y^2 = 4a^2  (semi-major 2a along x, semi-minor a along y)
E_a': rotated by theta counterclockwise, 0 < theta < 90 degrees

b = distance from origin to the two closer intersection points
c = distance from origin to the two farther intersection points
(a, b, c) is a canonical ellipsoidal triplet if a, b, c are positive integers.

C(N) = number of distinct canonical ellipsoidal triplets with a <= N.
Known: C(10^3) = 7, C(10^4) = 106, C(10^6) = 11845.
Answer: C(10^17) = 1199215615081353.

=== Mathematical Derivation ===

Ellipse E_a in polar form: r^2 = 4a^2 / (1 + 3 sin^2(phi))
E_a' rotated by theta: r^2 = 4a^2 / (1 + 3 sin^2(phi - theta))

Intersection => sin^2(phi) = sin^2(phi - theta) => phi = theta/2 or phi = theta/2 + pi/2.

Distances to origin:
  c^2 = 4a^2 / (1 + 3 sin^2(theta/2))     [farther pair, c > b]
  b^2 = 4a^2 / (1 + 3 cos^2(theta/2))     [closer pair]

Let s = sin^2(theta/2), 0 < s < 1/2. Then:
  c^2 = 4a^2 / (1 + 3s)
  b^2 = 4a^2 / (4 - 3s)

Eliminating s: 5 b^2 c^2 = 4 a^2 (b^2 + c^2)   ... (*)

Constraints: b < c < 2b (from 0 < s < 1/2).

=== Parametrization ===

Setting b = gp, c = gq with gcd(p,q) = 1 and p < q < 2p:
  5 g^2 p^2 q^2 = 4 a^2 (p^2 + q^2)

Since gcd(p,q)=1 => gcd(p^2 q^2, p^2+q^2) = 1.
So (p^2+q^2) | 5 g^2. Also 4a^2 = 5g^2 p^2 q^2 / (p^2+q^2).

For a to be an integer, we need p^2+q^2 = 5 s^2 for some integer s,
and then a = g p q / (2s) * s ... more precisely:

(2a)^2 = 5 (gpq)^2 / (p^2+q^2). If p^2+q^2 = 5s^2: (2a)^2 = (gpq)^2/s^2 => 2a = gpq/s.
Need s | gpq. Since gcd(p,q)=1 and s^2 = (p^2+q^2)/5, we have gcd(s,pq) might share factors.

Let e = gcd(s, pq). Then s = e*s', gcd(s', pq)=1.
Need s' | g. Write g = s' * f. Then:
  a = g*p*q / (2*s) = s'*f*p*q / (2*e*s') = f*p*q / (2*e).
  b = g*p = s'*f*p, c = g*q = s'*f*q.
Need 2e | f*p*q. Since p,q have different parity (coprime, sum of squares = 5s^2):
  p*q is even, so 2 | pq. Need e | f*pq/2... let's just handle it computationally.

=== Efficient Approach ===

We need coprime (p,q) with p < q < 2p satisfying p^2 + q^2 = 5 s^2.

All primitive solutions to X^2 + Y^2 = 5Z^2 can be generated via:
  Multiply Gaussian integer (m + ni) by (2+i) or (2-i), where m^2+n^2 = k^2 (Pythagorean).

Specifically, from Pythagorean triple generators (u,v) with u > v > 0, gcd(u,v)=1, u-v odd:
  m = u^2 - v^2, n = 2uv, k = u^2 + v^2 (or m = 2uv, n = u^2 - v^2)

Applying (2+i): X = 2m - n, Y = m + 2n, Z = k.
  => X = 2(u^2-v^2) - 2uv, Y = (u^2-v^2) + 4uv for the (m, 2uv) form
  More cleanly: for any Pythagorean (m,n,k), X=|2m-n|, Y=m+2n (or |2n-m|, n+2m).
  Then X^2 + Y^2 = 5(m^2+n^2) = 5k^2.

Also (2-i)(m+ni) = (2m+n) + (2n-m)i => X=2m+n, Y=|2n-m|.

So from each primitive Pythagorean triple (m,n,k), we get solutions (p,q,s) to p^2+q^2 = 5s^2.
"""

import math
from collections import defaultdict

def is_perfect_square(n):
    if n < 0:
        return False, 0
    if n == 0:
        return True, 0
    r = int(math.isqrt(n))
    if r * r == n:
        return True, r
    return False, 0

def brute_force_C(N):
    """Brute force for small N: check all (b,c) pairs."""
    triplets = set()
    for b in range(1, 2 * N + 1):
        for c in range(b + 1, min(2 * b, 2 * N + 1)):
            num = 5 * b * b * c * c
            den = 4 * (b * b + c * c)
            if num % den != 0:
                continue
            a_sq = num // den
            ok, a = is_perfect_square(a_sq)
            if ok and 0 < a <= N:
                triplets.add((a, b, c))
    return len(triplets), sorted(triplets)

def generate_primitive_pqs(max_s):
    """
    Generate all primitive solutions (p, q, s) with p^2 + q^2 = 5 s^2,
    gcd(p,q) = 1, p < q < 2p, s <= max_s.

    From Pythagorean triple (m, n, k) with m^2 + n^2 = k^2:
    Multiply (m + ni) by (2 + i):
      => (2m - n) + (m + 2n)i   gives X=2m-n, Y=m+2n, X^2+Y^2 = 5k^2, s=k
    Multiply (m + ni) by (2 - i):
      => (2m + n) + (2n - m)i   gives X=2m+n, Y=2n-m, X^2+Y^2 = 5k^2, s=k
    Also with (n + mi): X=2n-m, Y=n+2m and X=2n+m, Y=2m-n.
    """
    results = []

    # Enumerate primitive Pythagorean triples: m = u^2-v^2, n = 2uv, k = u^2+v^2
    # with u > v > 0, gcd(u,v)=1, u-v odd.
    max_k = max_s

    for u in range(2, int(math.isqrt(2 * max_k)) + 2):
        for v in range(1, u):
            if (u - v) % 2 == 0:
                continue
            if math.gcd(u, v) != 1:
                continue
            k = u * u + v * v
            if k > max_k:
                break
            m = u * u - v * v
            n = 2 * u * v

            # From (m + ni)(2 + i) = (2m-n) + (m+2n)i
            candidates = []

            # Type 1: (2m - n, m + 2n)
            X1 = 2 * m - n
            Y1 = m + 2 * n
            candidates.append((abs(X1), abs(Y1)))

            # Type 2: (2m + n, 2n - m)
            X2 = 2 * m + n
            Y2 = 2 * n - m
            candidates.append((abs(X2), abs(Y2)))

            # Type 3: (2n - m, n + 2m)
            X3 = 2 * n - m
            Y3 = n + 2 * m
            candidates.append((abs(X3), abs(Y3)))

            # Type 4: (2n + m, 2m - n)
            X4 = 2 * n + m
            Y4 = 2 * m - n
            candidates.append((abs(X4), abs(Y4)))

            for X, Y in candidates:
                if X <= 0 or Y <= 0:
                    continue
                p, q = min(X, Y), max(X, Y)
                # Check p < q < 2p
                if p >= q or q >= 2 * p:
                    continue
                g = math.gcd(p, q)
                p0, q0 = p // g, q // g
                # Verify: p0^2 + q0^2 should be 5 * (k/g_or_something)^2
                # Actually p^2 + q^2 = 5 k^2, and p = g*p0, q = g*q0
                # g^2(p0^2+q0^2) = 5k^2 => p0^2+q0^2 = 5k^2/g^2
                # Need g^2 | 5k^2. s = k * sqrt(5/g^2(p0^2+q0^2))...
                # Actually we know p^2 + q^2 = 5k^2 exactly. Verify:
                assert X*X + Y*Y == 5*k*k, f"{X}^2+{Y}^2={X*X+Y*Y} != 5*{k}^2={5*k*k}"

                # For primitive: gcd(p0, q0) = 1
                if g > 1:
                    # p, q not coprime; reduce
                    # p0^2 + q0^2 = 5*k^2/g^2, need this integer
                    if (5 * k * k) % (g * g) != 0:
                        continue
                    s0_sq = (5 * k * k) // (g * g) // 5  # wrong, let me redo
                    # p0^2 + q0^2 = 5*(k/g)^2 if g|k
                    # or p0^2 + q0^2 = 5*k^2/g^2 which might not factor nicely
                    # Just use (p0, q0) directly with s computed from p0^2+q0^2
                    h = p0*p0 + q0*q0
                    if h % 5 != 0:
                        continue
                    s_check_sq = h // 5
                    ok, s_val = is_perfect_square(s_check_sq)
                    if not ok:
                        continue
                    if s_val <= max_s and math.gcd(p0, q0) == 1:
                        results.append((p0, q0, s_val))
                else:
                    # g = 1, already coprime
                    s_val_sq = (p*p + q*q) // 5
                    ok, s_val = is_perfect_square(s_val_sq)
                    if ok and s_val <= max_s:
                        results.append((p, q, s_val))

    # Deduplicate
    return list(set(results))

def count_C_efficient(N):
    """
    Efficiently count C(N).

    For each primitive (p0, q0, s0) with p0^2+q0^2 = 5*s0^2, gcd(p0,q0)=1, p0 < q0 < 2p0:
    General solution: p = d*p0, q = d*q0, s = d*s0 for any d >= 1.
    Then b = g*p = g*d*p0, c = g*q = g*d*q0.
    And a = g*p*q / (2*s) = g*d*p0*q0*d*... wait.

    Let me redo from the equation. With b = g*p0, c = g*q0 (NOT scaling p0,q0 by d):
    p0^2 + q0^2 = 5*s0^2, gcd(p0,q0) = 1.

    5 g^2 p0^2 q0^2 = 4 a^2 (p0^2 + q0^2) = 4 a^2 * 5 s0^2
    => g^2 p0^2 q0^2 = 4 a^2 s0^2
    => a = g * p0 * q0 / (2 * s0)

    Need s0 | g * p0 * q0 and the result to be even-divisible.
    Let e = gcd(s0, p0 * q0). Since gcd(p0, q0) = 1, e = gcd(s0, p0) * gcd(s0/gcd(s0,p0), q0)... messy.

    Simpler: let f = s0 / gcd(s0, p0*q0). Then need f | g.
    And also need 2 | (g * p0 * q0 / s0), i.e., the result g*p0*q0/(s0) must be even.

    p0, q0 coprime, one even one odd (since p0^2+q0^2 = 5s0^2; if both odd,
    sum = 2 mod 4, 5s0^2 = 2 mod 4 => s0^2 = 2/5 mod 4...
    Actually 5*s0^2 mod 4: if s0 odd, 5*1=5=1 mod 4; if s0 even, 5*0=0 mod 4.
    p0^2+q0^2 with both odd: 1+1=2 mod 4. So 2 mod 4 = 5*s0^2 mod 4.
    If s0 odd: 5s0^2 = 5 mod 8 or 5*1 = 5 mod 8. p0^2+q0^2 = 2 mod 4. 5 mod 4 = 1. 2 != 1. Contradiction.
    So if both p0,q0 odd: no solution. Good, confirms different parity, p0*q0 even).

    Since p0*q0 is even, let's write p0*q0 = 2*T.
    a = g * 2T / (2 * s0) = g * T / s0.
    Need s0 | g*T. Let f = s0 / gcd(s0, T). Need f | g.
    So g = f * k for any k >= 1.
    a = f*k * T / s0 = k * T * f / s0 = k * T / (s0/f) = k * T / gcd(s0, T).

    Let A0 = T / gcd(s0, T) = p0*q0 / (2 * gcd(s0, T)).
    Hmm, let me just define: a = k * p0 * q0 / (2 * s0) * f ...

    Let me just compute directly:
    a = g * p0 * q0 / (2 * s0).
    Let r = gcd(p0 * q0, 2 * s0). Then the "primitive a" is p0*q0 / r (with g_min = 2*s0/r).
    g_min = 2*s0 / gcd(p0*q0, 2*s0).
    a_min = g_min * p0 * q0 / (2*s0) = p0*q0 / gcd(p0*q0, 2*s0).
    Then g = g_min * k, a = a_min * k. a <= N => k <= N / a_min.
    """
    count = 0
    triplets = []

    # We need max_s. From a = g*p0*q0/(2*s0) <= N, and g >= g_min >= 1:
    # s0 <= p0*q0*g/(2) / a * ... rough: s0 ~ sqrt((p0^2+q0^2)/5) ~ p0*sqrt(2/5).
    # p0 < q0 < 2p0, so s0 ~ p0 * sqrt(1 + q0^2/p0^2)/sqrt(5) ~ p0 * sqrt(5/5) = p0.
    # Actually s0^2 = (p0^2+q0^2)/5, and with q0 ~ 1.5 p0: s0 ~ p0*sqrt(3.25/5) ~ 0.806 p0.
    # From a_min >= 1 and a_min = p0*q0/gcd(p0*q0,2s0) >= p0*q0/(2*s0) ~ p0*1.5p0/(2*0.8p0) ~ p0.
    # So p0 <= N, s0 <= N.

    max_s = int(2 * N) + 1  # generous upper bound

    primitives = generate_primitive_pqs(max_s)

    for p0, q0, s0 in primitives:
        # a = g * p0 * q0 / (2 * s0), need a positive integer.
        pq = p0 * q0
        two_s = 2 * s0
        r = math.gcd(pq, two_s)
        g_min = two_s // r
        a_min = pq // r

        # Verify
        assert g_min * pq == a_min * two_s, f"Consistency check failed for ({p0},{q0},{s0})"
        # a = a_min * k, g = g_min * k, b = g*p0 = g_min*k*p0, c = g*q0 = g_min*k*q0

        max_k = N // a_min
        for k in range(1, max_k + 1):
            a_val = a_min * k
            b_val = g_min * k * p0
            c_val = g_min * k * q0
            assert b_val < c_val < 2 * b_val, f"Range check: {b_val} < {c_val} < {2*b_val}"
            count += 1
            if N <= 10000:
                triplets.append((a_val, b_val, c_val))

    if triplets:
        triplets.sort()
    return count, triplets

def count_C_fast(N):
    """
    Fast counting of C(N) by directly enumerating Pythagorean generators.

    For each primitive Pythagorean triple generator (u, v) with u > v > 0,
    gcd(u,v)=1, u-v odd, k = u^2 + v^2:

    We produce candidate (p, q) from the Gaussian integer multiplication,
    filter for p < q < 2p and gcd(p,q)=1, compute a_min, and count multiples.
    """
    count = 0
    triplets = []

    # k = u^2 + v^2 <= max_s. We need max_s.
    # From the analysis, s0 = k (for primitive solutions where g_common = 1).
    # a_min ~ p0*q0/(2*s0). With p0 ~ 2k, q0 ~ 2k roughly:
    # a_min ~ 4k^2/(2k) = 2k. So k <= N/2 roughly.
    # But some solutions have smaller a_min. Let's use k <= N.

    max_k = N  # k = u^2 + v^2 can be at most this

    seen = set()

    for u in range(2, int(math.isqrt(max_k)) + 2):
        if u * u + 1 > max_k:
            break
        for v in range(1, u):
            if (u - v) % 2 == 0:
                continue
            if math.gcd(u, v) != 1:
                continue
            k = u * u + v * v
            if k > max_k:
                break

            m = u * u - v * v
            n = 2 * u * v

            # Generate all 4 candidate (X, Y) pairs
            candidates = [
                (2*m - n, m + 2*n),
                (2*m + n, 2*n - m),
                (2*n - m, n + 2*m),
                (2*n + m, 2*m - n),
            ]

            for X, Y in candidates:
                X, Y = abs(X), abs(Y)
                if X == 0 or Y == 0:
                    continue
                p, q = min(X, Y), max(X, Y)

                if q >= 2 * p:  # need p < q < 2p
                    continue

                g = math.gcd(p, q)
                p0, q0 = p // g, q // g

                if (p0, q0) in seen:
                    continue

                # Verify p0^2 + q0^2 = 5 * s0^2
                h = p0 * p0 + q0 * q0
                if h % 5 != 0:
                    continue
                s0_sq = h // 5
                ok, s0 = is_perfect_square(s0_sq)
                if not ok:
                    continue

                seen.add((p0, q0))

                # Compute a_min and g_min
                pq = p0 * q0
                two_s = 2 * s0
                r = math.gcd(pq, two_s)
                g_min = two_s // r
                a_min = pq // r

                max_mult = N // a_min
                count += max_mult
                if N <= 10000:
                    for kk in range(1, max_mult + 1):
                        triplets.append((a_min * kk, g_min * kk * p0, g_min * kk * q0))

    if triplets:
        triplets.sort()
    return count, triplets

def verify_example():
    """Verify the example triplet (209, 247, 286)."""
    a, b, c = 209, 247, 286
    lhs = 5 * b**2 * c**2
    rhs = 4 * a**2 * (b**2 + c**2)
    print(f"Checking (a,b,c) = ({a},{b},{c}):")
    print(f"  5*b^2*c^2 = {lhs}")
    print(f"  4*a^2*(b^2+c^2) = {rhs}")
    print(f"  Equal: {lhs == rhs}")
    print(f"  b < c < 2b: {b} < {c} < {2*b} => {b < c < 2*b}")

    s = (4*b**2 - c**2) / (3*(b**2 + c**2))
    theta = 2 * math.asin(math.sqrt(s))
    print(f"  sin^2(theta/2) = {s:.6f}")
    print(f"  theta = {math.degrees(theta):.4f} degrees")
    print()

def visualize_crisscross_ellipses(a=5, theta_deg=60, filename="visualization.png"):
    """Visualize crisscross ellipse patterns."""
