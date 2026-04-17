"""
Problem 496: Incenter and Circumcenter of Triangle
For triangles with integer sides, study when OI^2 is a perfect square.
OI^2 = R(R - 2r) where R = circumradius, r = inradius.
"""

from math import gcd, isqrt

def is_perfect_square(n: int) -> bool:
    """Check if n is a perfect square (n >= 0)."""
    if n < 0:
        return False
    if n == 0:
        return True
    r = isqrt(n)
    return r * r == n

def compute_OI_squared(a: int, b: int, c: int):
    """Compute OI^2 for triangle with sides a, b, c.
    Returns (numerator, denominator) of OI^2 in lowest terms,
    or None if the triangle is degenerate.
    """
    # Semi-perimeter (times 2 to avoid fractions)
    s2 = a + b + c  # 2s
    if s2 % 2 != 0:
        # s is not integer; work with doubled quantities
        # s = s2/2, s-a = (s2 - 2a)/2, etc.
        sa2 = s2 - 2 * a  # 2(s-a)
        sb2 = s2 - 2 * b
        sc2 = s2 - 2 * c
    else:
        sa2 = s2 - 2 * a
        sb2 = s2 - 2 * b
        sc2 = s2 - 2 * c

    if sa2 <= 0 or sb2 <= 0 or sc2 <= 0:
        return None  # degenerate

    # 16 * Delta^2 = s2 * sa2 * sb2 * sc2 (when using 2s, 2(s-a), etc.)
    # Actually: 16*Delta^2 = 2s * 2(s-a) * 2(s-b) * 2(s-c) / 1 -- no.
    # Heron: Delta^2 = s(s-a)(s-b)(s-c)
    # = (s2/2)(sa2/2)(sb2/2)(sc2/2) = s2*sa2*sb2*sc2 / 16
    # So 16*Delta^2 = s2*sa2*sb2*sc2

    delta2_times_16 = s2 * sa2 * sb2 * sc2

    # OI^2 = R^2 - 2Rr
    # R = abc / (4*Delta), r = Delta / s = 2*Delta / s2
    # R^2 = a^2*b^2*c^2 / (16*Delta^2)
    # 2Rr = 2 * abc/(4*Delta) * Delta/s = abc/(2s) = abc*2/(2*s2) = abc/s2
    # Wait: 2Rr = 2 * (abc/(4*Delta)) * (2*Delta/s2) = abc/s2

    # OI^2 = a^2*b^2*c^2 / (16*Delta^2) - abc/s2
    # = a^2*b^2*c^2 / (s2*sa2*sb2*sc2/1) -- no.
    # Since 16*Delta^2 = s2*sa2*sb2*sc2:
    # OI^2 = a^2*b^2*c^2*16 / (16 * s2*sa2*sb2*sc2) -- no:
    # R^2 = a^2*b^2*c^2 / (16*Delta^2) = a^2*b^2*c^2 / (s2*sa2*sb2*sc2/1)
    # Hmm, let me redo carefully.

    # Delta^2 = s2*sa2*sb2*sc2 / 16
    # R^2 = (abc)^2 / (16 * Delta^2) = (abc)^2 * 16 / (16 * s2*sa2*sb2*sc2) = (abc)^2 / (s2*sa2*sb2*sc2)

    # 2Rr: r = Delta/s, R = abc/(4*Delta)
    # 2Rr = 2*abc/(4*Delta) * Delta/s = abc/(2s) = abc*2/(2*s2) --- no: 2s = s2, so s = s2/2
    # 2Rr = abc / (2 * s2/2) = abc / s2 --- no:
    # 2Rr = abc / (2s) = abc / (s2/1) --- 2s = s2, so 2Rr = abc/s2 ... wait
    # 2Rr = abc / (2 * s) and s = s2/2, so 2Rr = abc / (2 * s2/2) = abc/s2

    # Hmm, let me just be careful: s = (a+b+c)/2 = s2/2
    # 2Rr = abc / (2s) = abc / s2
    # No. 2Rr = 2 * R * r.
    # R = abc/(4*Delta), r = Delta/s
    # 2*R*r = 2 * abc/(4*Delta) * Delta/s = 2*abc/(4*s) = abc/(2s)
    # 2s = s2, so abc/(2s) = abc/s2

    # OI^2 = R^2 - 2Rr = (abc)^2 / (s2*sa2*sb2*sc2) - abc/s2

    # Common denominator: s2*sa2*sb2*sc2
    # OI^2 = [(abc)^2 - abc*sa2*sb2*sc2] / (s2*sa2*sb2*sc2)
    # = abc * [abc - sa2*sb2*sc2] / (s2*sa2*sb2*sc2)

    abc = a * b * c
    numer = abc * (abc - sa2 * sb2 * sc2)
    denom = s2 * sa2 * sb2 * sc2

    if numer <= 0:
        return None  # OI^2 <= 0

    g = gcd(abs(numer), abs(denom))
    return (numer // g, denom // g)

def is_rational_perfect_square(num: int, den: int) -> bool:
    """Check if num/den is a perfect square of a rational number."""
    # num/den = (p/q)^2 iff num*den is a perfect square (when num/den is in lowest terms)
    # Actually: if num/den = (p/q)^2 in lowest terms, then num = p^2 and den = q^2
    return is_perfect_square(num) and is_perfect_square(den)

def solve(N: int = 100):
    """Count triangles with 1 <= a <= b <= c <= N where OI^2 is a perfect square."""
    count = 0
    results = []
    for a in range(1, N + 1):
        for b in range(a, N + 1):
            c_min = b
            c_max = min(N, a + b - 1)  # triangle inequality: c < a + b
            for c in range(c_min, c_max + 1):
                result = compute_OI_squared(a, b, c)
                if result is not None:
                    num, den = result
                    if is_rational_perfect_square(num, den):
                        count += 1
                        if N <= 50:
                            results.append((a, b, c, num, den))
    if results:
        print("Triangles with OI^2 a perfect square:")
        for a, b, c, num, den in results[:20]:
            print(f"  ({a}, {b}, {c}): OI^2 = {num}/{den}")
    return count

# Compute for small values
for n in [20, 50, 100]:
    count = solve(n)
    print(f"T({n}) = {count}\n")
