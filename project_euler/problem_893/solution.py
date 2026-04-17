"""
Problem 893: Steiner Systems
S(t,k,v): every t-subset in exactly one k-block.
"""

from math import comb
from itertools import combinations

def steiner_params(t, k, v):
    """Compute b (blocks) and r (replication) for S(t,k,v)."""
    b_num = comb(v, t)
    b_den = comb(k, t)
    if b_den == 0 or b_num % b_den != 0:
        return None, None
    b = b_num // b_den
    r_num = comb(v - 1, t - 1)
    r_den = comb(k - 1, t - 1)
    if r_den == 0 or r_num % r_den != 0:
        return None, None
    r = r_num // r_den
    return b, r

def check_necessary(t, k, v):
    """Check all necessary divisibility conditions."""
    for i in range(t + 1):
        num = comb(v - i, t - i)
        den = comb(k - i, t - i)
        if den == 0 or num % den != 0:
            return False
    return True

def fano_plane():
    """Return the Fano plane S(2,3,7)."""
    return [{1,2,4}, {2,3,5}, {3,4,6}, {4,5,7}, {5,6,1}, {6,7,2}, {7,1,3}]

def verify_steiner(blocks, t, v):
    """Verify that blocks form an S(t,k,v)."""
    points = set(range(1, v + 1))
    for subset in combinations(points, t):
        subset_set = set(subset)
        count = sum(1 for B in blocks if subset_set <= B)
        if count != 1:
            return False, subset
    return True, None

# --- Verification ---
print("=== Steiner System Parameters ===")
systems = [(2,3,7), (2,3,9), (2,3,13), (3,4,8), (4,5,11), (5,6,12), (5,8,24)]
for t, k, v in systems:
    b, r = steiner_params(t, k, v)
    nec = check_necessary(t, k, v)
    print(f"  S({t},{k},{v}): b={b}, r={r}, necessary={'OK' if nec else 'FAIL'}")

print("\n=== Fano Plane Verification ===")
blocks = fano_plane()
ok, conflict = verify_steiner(blocks, 2, 7)
print(f"  S(2,3,7) valid: {'OK' if ok else f'FAIL at {conflict}'}")
assert ok
print(f"  Blocks: {[sorted(b) for b in blocks]}")

print("\n=== Steiner Triple Systems S(2,3,v) ===")
for v in range(3, 40):
    b, r = steiner_params(2, 3, v)
    if b is not None and check_necessary(2, 3, v):
        print(f"  v={v:>2}: b={b:>3}, r={r:>2}, v mod 6 = {v % 6}")

print("\n=== Fisher Inequality Check ===")
for t, k, v in systems:
    b, r = steiner_params(t, k, v)
    fisher = b >= v
    print(f"  S({t},{k},{v}): b={b} >= v={v}: {'OK' if fisher else 'FAIL'}")
    assert fisher

answer = steiner_params(5, 8, 24)
print(f"\nAnswer: S(5,8,24) has b={answer[0]} blocks, r={answer[1]} per point")

# --- 4-Panel Visualization ---
