"""
Problem 942: Polygonal Number Chains

Investigate multi-polygonal numbers up to a limit. A number is s-gonal if
    P(s, k) = k * ((s-2)*k - (s-4)) / 2
for some positive integer k, with s >= 3 (triangle, square, pentagonal, ...).

A multi-polygonal number belongs to two or more polygonal families
(e.g., 1 is triangular, square, pentagonal, ...).

Results:
    - Number of multi-polygonal values up to 10000 computed below
    - Distribution by number of polygon types shown in visualization

Methods:
    1. polygonal          -- compute P(s, k)
    2. build_poly_map     -- map each value to its set of polygon types
    3. find_multi         -- filter to values with >= 2 types
    4. is_s_gonal        -- check if a value is s-gonal (verification)
"""
from collections import defaultdict, Counter

def polygonal(s, k):
    """Compute the k-th s-gonal number: P(s,k) = k*((s-2)*k - (s-4)) / 2."""
    return k * ((s - 2) * k - (s - 4)) // 2

def build_poly_map(limit, s_range=range(3, 9)):
    """Map each polygonal value (up to limit) to its set of s-types."""
    poly_map = defaultdict(set)
    for s in s_range:
        k = 1
        while True:
            v = polygonal(s, k)
            if v > limit:
                break
            poly_map[v].add(s)
            k += 1
    return poly_map

def find_multi(poly_map, min_types=2):
    """Return {value: types} for values with >= min_types polygon families."""
    return {v: types for v, types in poly_map.items() if len(types) >= min_types}

def is_s_gonal(val, s):
    """Check if val = P(s, k) for some positive integer k."""
    # Solve k*((s-2)*k - (s-4))/2 = val  =>  (s-2)*k^2 - (s-4)*k - 2*val = 0
    a = s - 2
    b = -(s - 4)
    c = -2 * val
    disc = b * b - 4 * a * c
    if disc < 0:
        return False
    sqrt_disc = int(disc**0.5)
    if sqrt_disc * sqrt_disc != disc:
        return False
    num = -b + sqrt_disc
    den = 2 * a
    return num > 0 and num % den == 0

# Verification
assert polygonal(3, 1) == 1    # T_1 = 1
assert polygonal(3, 4) == 10   # T_4 = 10
assert polygonal(4, 3) == 9    # square_3 = 9
assert polygonal(5, 2) == 5    # P5_2 = 5
assert is_s_gonal(1, 3) and is_s_gonal(1, 4) and is_s_gonal(1, 5)  # 1 is multi
assert is_s_gonal(36, 3) and is_s_gonal(36, 4)  # 36 is triangular and square

# Computation
limit = 10000
poly_map = build_poly_map(limit)
multi = find_multi(poly_map)
chain = sorted(multi.keys())

answer = len(multi)
print(answer)
