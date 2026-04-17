"""
Problem 510: Tangent Circles
Three mutually tangent circles also tangent to a line.
Descartes Circle Theorem gives the constraint:
    a^2*b^2 + b^2*c^2 + a^2*c^2 = 2*a*b*c*(a + b + c)
Find sum of (a+b+c) over valid integer triples with a <= b <= c, a+b+c <= N.
"""

from math import gcd, isqrt

def is_perfect_square(n: int) -> bool:
    if n < 0:
        return False
    s = isqrt(n)
    return s * s == n

def check_descartes(a: int, b: int, c: int) -> bool:
    """Check if (a, b, c) satisfies the tangent circle equation."""
    lhs = a*a*b*b + b*b*c*c + a*a*c*c
    rhs = 2*a*b*c*(a + b + c)
    return lhs == rhs

def find_solutions_parametric(N: int) -> list:
    """
    Find all triples (a, b, c) with a <= b <= c, a+b+c <= N satisfying
    a^2*b^2 + b^2*c^2 + a^2*c^2 = 2*a*b*c*(a+b+c).

    Parameterization: a = d*u^2, b = d*v^2 (gcd(u,v) can be anything)
    c = d*u^2*v^2 / (u-v)^2  or  c = d*u^2*v^2 / (u+v)^2

    We enumerate d, u, v.
    """
    solutions = set()

    # Using c = d*u^2*v^2 / (u +/- v)^2
    # with a = d*u^2, b = d*v^2, u > v >= 1

    max_d = N  # a = d*1 at minimum
    for u in range(1, isqrt(N) + 2):
        for v in range(1, u + 1):
            # Case 1: denominator = (u - v)^2
            if u > v:
                denom1 = (u - v) ** 2
                # c = d * u^2 * v^2 / denom1, need denom1 | d * u^2 * v^2
                # a = d*u^2, b = d*v^2, a <= b means u <= v (but u > v here)
                # so a = d*u^2 >= d*v^2 = b, meaning b <= a
                # We want a <= b <= c, so sort the triple

                # For c to be integer: denom1 | d * u^2 * v^2
                # Let g = gcd(u^2 * v^2, denom1)
                # Need denom1/g | d
                g1 = gcd(u * u * v * v, denom1)
                step1 = denom1 // g1  # d must be multiple of step1

                for d in range(step1, N + 1, step1):
                    a_val = d * u * u
                    b_val = d * v * v
                    c_val = d * u * u * v * v // denom1

                    if c_val <= 0:
                        continue

                    triple = tuple(sorted([a_val, b_val, c_val]))
                    if triple[0] + triple[1] + triple[2] > N:
                        if d == step1:
                            break
                        continue
                    if triple[0] >= 1:
                        # Verify
                        if check_descartes(*triple):
                            solutions.add(triple)

            # Case 2: denominator = (u + v)^2
            denom2 = (u + v) ** 2
            g2 = gcd(u * u * v * v, denom2)
            step2 = denom2 // g2

            for d in range(step2, N + 1, step2):
                a_val = d * u * u
                b_val = d * v * v
                c_val = d * u * u * v * v // denom2

                if c_val <= 0:
                    continue

                triple = tuple(sorted([a_val, b_val, c_val]))
                if triple[0] + triple[1] + triple[2] > N:
                    if d == step2:
                        break
                    continue
                if triple[0] >= 1:
                    if check_descartes(*triple):
                        solutions.add(triple)

    return sorted(solutions)

def find_solutions_brute(N: int) -> list:
    """Brute-force search for small N."""
    solutions = []
    for a in range(1, N + 1):
        for b in range(a, N - a + 1):
            for c in range(b, N - a - b + 1):
                if check_descartes(a, b, c):
                    solutions.append((a, b, c))
    return solutions

# Small brute-force verification
N_small = 100
sols_bf = find_solutions_brute(N_small)
print(f"Brute-force solutions with a+b+c <= {N_small}:")
for s in sols_bf:
    print(f"  {s}, sum = {sum(s)}")

total_bf = sum(sum(s) for s in sols_bf)
print(f"Total sum: {total_bf}")

# Parametric search
sols_param = find_solutions_parametric(N_small)
print(f"\nParametric solutions with a+b+c <= {N_small}:")
for s in sols_param:
    print(f"  {s}, sum = {sum(s)}")

total_param = sum(sum(s) for s in sols_param)
print(f"Total sum: {total_param}")

# Larger computation
N_large = 1000
sols_large = find_solutions_parametric(N_large)
total_large = sum(sum(s) for s in sols_large)
print(f"\nSolutions with a+b+c <= {N_large}: {len(sols_large)} triples")
print(f"Sum of (a+b+c): {total_large}")
