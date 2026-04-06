"""
Project Euler Problem 390: Triangles with Non-Rational Sides and Integral Area

Triangle sides: sqrt(1+b^2), sqrt(1+c^2), sqrt(b^2+c^2) for positive integers b, c.

Using Heron's formula (or cross-product in 3D embedding), the area is:
    Area = sqrt(b^2*c^2 + b^2 + c^2) / 2

For Area to be a positive integer:
  1) b^2*c^2 + b^2 + c^2 must be a perfect square m^2, and
  2) m must be even.
Both conditions require b and c to be even.

Writing b = 2B, c = 2C:
    Area = sqrt(4B^2*C^2 + B^2 + C^2)  (must be a positive integer)

Equivalently, (b^2+1)(c^2+1) = (2*Area)^2 + 1.

S(n) = sum of Area over all positive integer pairs (b, c) where Area is a
positive integer not exceeding n.

Algorithm: Generalized Pell Equation Enumeration
-------------------------------------------------
For each even value a (the smaller side parameter), we solve:
    v^2 - (a^2+1)*t^2 = -a^2

The fundamental solution of x^2 - (a^2+1)*y^2 = 1 is (2*a^2+1, 2*a).

Orbit representatives are found by checking even t0 in [2, a] for which
(a^2+1)*t0^2 - a^2 is a perfect square v0^2.

For non-trivial reps (t0 < a): both +v0 and -v0 seeds produce distinct orbits.
For the trivial rep (t0 = a, v0 = a^2): only one orbit (the +v0/-v0 orbits coincide).

From each solution (v, t): b_param = a*t + v, n_double = a*b_param + t, Area = n_double/2.

Answer: S(10^10) = 2919133642971
"""

import math
import time

def solve(limit, collect_triples=True):
    """
    Compute S(limit) using generalized Pell equation enumeration.

    Returns:
        (answer, count, triples) where triples is a list of (a, b, area).
    """
    N = 2 * limit
    ans = 0
    count = 0
    triples = []
    a_max = int(math.isqrt(N))

    for a in range(2, a_max + 1, 2):
        D = a * a + 1
        a2 = a * a
        p = 2 * a2 + 1   # fundamental Pell x-component
        q = 2 * a         # fundamental Pell y-component

        for t0 in range(2, a + 1, 2):
            v2 = D * t0 * t0 - a2
            if v2 <= 0:
                continue
            v0 = int(math.isqrt(v2))
            if v0 * v0 != v2:
                continue

            # Non-trivial reps produce two distinct orbits (+v0 and -v0).
            # Trivial rep (t0 == a) produces one orbit (they coincide).
            signs = [v0, -v0] if t0 < a else [v0]

            for v_start in signs:
                v_cur, t_cur = v_start, t0
                while True:
                    if v_cur > 0 and t_cur > 0:
                        b_param = a * t_cur + v_cur
                        nn = a * b_param + t_cur
                        if nn <= N:
                            area = nn // 2
                            ans += area
                            count += 1
                            triples.append((a, b_param, area))
                        else:
                            break
                    # Pell iteration
                    v_next = v_cur * p + D * t_cur * q
                    t_next = v_cur * q + t_cur * p
                    if t_next > N * 100:
                        break
                    v_cur, t_cur = v_next, t_next

    return ans, count, triples

def solve_brute(limit):
    """Brute-force solution for verification (slow, only for small limit)."""
    N = 2 * limit
    ans = 0
    for a in range(2, int(math.isqrt(N)) + 1, 2):
        if a * a + 1 > N:
            break
        upper = N // (a * a + 1)
        for t in range(2, int(upper) + 1, 2):
            s = a * a * t * t - a * a + t * t
            v = int(math.isqrt(s))
            if v * v == s:
                b = a * t + v
                n_val = a * b + t
                if n_val > N:
                    break
                ans += n_val // 2
    return ans

def create_visualization(triples, limit, save_path):
    """Create and save a visualization of the solution."""
